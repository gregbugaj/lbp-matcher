#include <sstream>
#include <nmmintrin.h>
#include "leptonutil.h"

bool debug_pix = false;

void dump(PIX* pix)
{
    int_t w = pix->w;
    int_t h = pix->h;

    int_t wpl = pixGetWpl(pix);
    l_uint32* data = pixGetData(pix);
    l_uint32* line;

    printf("\n");
    printf("Depth : %d \n", pix->d);

    for (int_t y = 0; y < h; ++y)
    {
        printf("%04d  :  ", y);
        line = data + y * wpl;
        for (int_t x = 0; x < w; ++x)
        {
            l_uint32 val = 0;

            if(pix->d == 1)
                val = GET_DATA_BIT(line, x);
            else if(pix->d == 2)
                val = GET_DATA_DIBIT(line, x);
            else if(pix->d == 4)
                val = GET_DATA_QBIT(line, x);
            else // 8, 16, 32
                val = GET_DATA_BYTE(line, x);

            printf("%03d ", val);
        }
        printf("\n");
    }
}

l_uint32 sum(PIX* pix)
{
    if(pix->d != 8)
    {
        throw std::runtime_error(std::string("PIX should be 8bpp, got : "+pix->d));
    }

    l_int32 w = pix->w;
    l_int32 h = pix->h;

    l_int32 wpl     = pixGetWpl(pix);
    l_uint32* data = pixGetData(pix);
    l_uint32* line;
    l_uint32  sum = 0;

    for (int_t y = 0; y < h; ++y)
    {
        line = data + y * wpl;
        for (int_t x = 0; x < w; ++x)
        {
            sum += GET_DATA_BYTE(line, x);
        }
    }

    return sum;
}

BOX* boxaBoundingRegion(BOXA* boxes)
{
    BOX* box = NULL;
    auto count = boxaGetCount(boxes);
    for(auto n = 0; n < count; ++n)
    {
        BOX* current = boxaGetBox(boxes, n, L_COPY);
        if(box != NULL)
            box = boxBoundingRegion(current, box);
        else
            box = current;
    }
    return box;
}

/**
 * Get Pixel value at given  point
 */
l_uint32 pixAtGet(PIX* pix, int_t x, int_t y)
{
    l_int32 wpl = pixGetWpl(pix);
    l_uint32* data = pixGetData(pix);
    l_uint32* line = data + y * wpl;
    l_uint32 value = GET_DATA_BYTE(line, x);

    return value;
}

/**
 * Set Pixel value at given  point
 */
void pixAtSet(PIX* pix, int_t x, int_t y, byte_t value)
{
    l_int32 wpl = pixGetWpl(pix);
    l_uint32* data = pixGetData(pix);
    l_uint32* line = data + y * wpl;

    SET_DATA_BYTE(line, x, value);
}

PIX* pixUpscaleToGray(const char* filename)
{
    auto pix = pixRead(filename);
    return pixUpscaleToGray(pix);
}

PIX* pixUpscaleToGray(PIX* pix)
{
   if(pix->d != 1)
       return pix;

    PIX* gray = pixCopy(NULL, pix);
    gray = pixConvertTo8(gray, 0);
    gray = pixScaleGray2xLI(gray);
    gray = pixBlockconv(gray, 1, 1);
    gray = pixScaleAreaMap2(gray);

    return gray;
}

PIX* pixClipToBoundingBox(PIX* pix)
{
    // debug
    static int counter = 0;
    counter++;

    if(pix == nullptr)
        return nullptr;

    PIX* one = pixConvertTo1(pix, 127);
    BOXA* boxes = pixConnComp(one, NULL, 4);
    BOX* box = boxaBoundingRegion(boxes);

    // there were not connected components so lets default box to w/h
    if(box == nullptr)
        box = boxCreate(0,0, pix->w, pix->h);

    PIX* clipped = pixClipRectangle(pix, box, NULL);

    boxDestroy(&box);
    pixDestroy(&one);
    boxaDestroy(&boxes);

    return clipped;
}

PIX* normalize(PIX *pix)
{
    static int counter = 0;
    counter++;
    PIX* gray = NULL;

    if (pix->d == 1)
    {
        gray = pixUpscaleToGray(pix); // -- CAUSES TOO MANY ARTIFACTS
    }
    else if (pix->d == 8)
    {
        gray = pixConvertTo8(pix, 0);
    }
    else if (pix->d == 32)
    {
        gray = pixConvertTo8(pix, 0);
    /*    // convert color(32) to grayscale
        PIX* pdata = pixConvertTo8(pix, 0);
        //   pdata = pixThreshold8(pdata, 8, 32, 0);
        pdata = reduce(pix, 400, 0);
        pdata = pixConvertTo8(pdata, 0);
        //    pdata = reduce(pdata, 500, 300);*/
    }
    else
    {
        // Grayscale 8bpp/ Color 16/32 bpp
        gray = pixConvertTo8(pix, 0);
    }

    if (debug_pix)
    {
        char f1[255];
        sprintf(f1, "/tmp/lbp-matcher/norm-pix-%d.png", counter);
        pixWritePng(f1, pix, 0);
        sprintf(f1, "/tmp/lbp-matcher/norm-gray-%d.png", counter);
        pixWritePng(f1, gray, 0);
    }

    return gray;
}

int_t pixCountPixelsFast(PIX* pix)
{
    if(!pix)
        return 0;

    l_uint32* p   = pix->data;
    l_uint32* e   = p + (pix->wpl * pix->h);
    int_t   cnt = 0;

    for (; p != e; p++)
    {
        int_t x = _mm_popcnt_u32(x);
        cnt += x;
    }
    return cnt;
}

/**
 * Smoothing and converting binary image to grayscale image
 *
 * @param pix the pix to normalize
 * @return normalized pix
 */
PIX* normalizeOLD(PIX *pix, int padLeft, int padRight, int padTop, int padBottom) {
    static int counter = 0;
    counter++;

    padTop = 0;
    padBottom = 0;
    padLeft = 0;
    padRight = 0;

    PIX* gray = NULL;

    if (pix->d == 1)
    {
        gray = pixUpscaleToGray(pix); // -- CAUSES TOO MANY ARTIFACTS
    }
    else if (pix->d == 8)
    {
        gray = pixConvertTo8(pix, 0);
    }
    else if (pix->d == 32)
    {
        //throw std::runtime_error("Conversion PIX BPP of 32 is not yet implemented");
        gray = pixConvertTo8(pix, 0);
    /*    // convert color(32) to grayscale
        PIX* pdata = pixConvertTo8(pix, 0);
        //   pdata = pixThreshold8(pdata, 8, 32, 0);
        pdata = reduce(pix, 400, 0);
        pdata = pixConvertTo8(pdata, 0);
        //    pdata = reduce(pdata, 500, 300);*/

    }
    else
    {
        // Grayscale 8bpp/ Color 16/32 bpp
         gray = pixConvertTo8(pix, 0);
    }

    PIX* one = pixConvertTo1(gray, 127);
    BOXA* boxes = pixConnComp(one, NULL, 4);
    BOX* box = boxaBoundingRegion(boxes);

    // there were not connected components so lets default box to w/h
    if(box == nullptr)
    {
        char f1[255];

        sprintf(f1, "/tmp/lbp-matcher/bad-%d.png", counter);
        pixWritePng(f1, pix, 0);

        box = boxCreate(0,0, pix->w, pix->h);
        //throw std::runtime_error("Bad box ");
    }

    PIX* clipped = pixClipRectangle(gray, box, NULL);
    PIX* padded  = pixCreate(box->w + (padLeft + padRight), box->h + (padBottom + padTop), 8);
    pixSetResolution(padded, 300, 300);
//    pixSetAll(padded);
//    pixSetBlackOrWhite(padded, L_SET_BLACK);
//    l_int32 status = pixRasterop(padded, padLeft, padTop, padded->w, padded->h, PIX_SRC | PIX_DST, clipped, 0, 0);
//        l_int32 status = pixRasterop(padded, padLeft, padTop, padded->w, padded->h, PIX_SRC ^ PIX_DST, clipped, 0, 0);
//        l_int32 status = pixRasterop(padded, padLeft, padTop, padded->w, padded->h, PIX_DST, clipped, 0, 0);
//        l_int32 status = pixRasterop(padded, padLeft, padTop, clipped->w, clipped->h, PIX_SRC | PIX_DST , clipped, 0, 0);
//    pixWritePng("/tmp/lbp-matcher/clipped-pdata.png", clipped, 1);
//    pixWritePng("/tmp/lbp-matcher/padded-pdata.png", padded, 1);

    if (debug_pix)
    {
        char f1[255];

        sprintf(f1, "/tmp/lbp-matcher/norm-pix-%d.png", counter);
        pixWritePng(f1, pix, 0);

        sprintf(f1, "/tmp/lbp-matcher/norm-gray-%d.png", counter);
        pixWritePng(f1, gray, 0);

        sprintf(f1, "/tmp/lbp-matcher/norm-padded-%d.png", counter);
        pixWritePng(f1, padded, 0);

        sprintf(f1, "/tmp/lbp-matcher/norm-clipped-%d.png", counter);
        pixWritePng(f1, clipped, 0);
    }

//    pixDestroy(&clipped);
    pixDestroy(&one);
    pixDestroy(&gray);
    boxaDestroy(&boxes);
    boxDestroy(&box);

    return clipped;
}

PIX* reduce(PIX *pix, int width, int height)
{
    return pixScaleToSize(pix, width, height);
}

void pixFromMatrix(int **matrix, int rows, int cols, char *filename)
{
    PIX* pix = pixFromMatrix(matrix, rows, cols);
    pixWritePng(filename, pix, 1);
    pixDestroy(&pix);
}

PIX* pixFromMatrix(int **matrix, int rows, int cols)
{
    int w = cols;
    int h = rows;
    PIX* pix = pixCreate(w, h, 8);
    pixSetResolution(pix, 300, 300);
    l_int32 wpl = pixGetWpl(pix);
    l_uint32* data = pixGetData(pix);

    for (int_t y = 0; y < h; ++y)
    {
        l_uint32* line = data + y * wpl;
        for (int_t x = 0; x < w; ++x)
        {
            SET_DATA_BYTE(line, x, matrix[y][x]);
        }
    }
    return pix;
}