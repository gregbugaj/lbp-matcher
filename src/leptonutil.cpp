#include <sstream>
#include "leptonutil.h"

bool debug_pix = true;

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

    int_t w = pix->w;
    int_t h = pix->h;

    int_t wpl      = pixGetWpl(pix);
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

PIX* create_grayscale(PIX* pix)
{
    PIX* gray = pixCopy(NULL, pix);

    gray = pixConvert1To8(NULL, gray, 255, 0);
    gray = pixScaleGray2xLI(gray);
    gray = pixBlockconv(gray, 1, 1);
    gray = pixScaleAreaMap2(gray);

    return gray;
}

/**
 * Smoothing and converting binary image to grayscale image
 *
 * @param pix the pix to normalize
 * @return normalized pix
 */
PIX* normalize(PIX *pix, int padLeft, int padRight, int padTop, int padBottom) {
    static int counter = 0;
    counter++;

    if (pix->d == 1)
    {
        //PIX*  gray     = pixConvertTo8(pix, NULL); //create_grayscale(pix);
        PIX *gray = create_grayscale(pix); // -- CAUSES TOO MANY ARTIFACTS
        BOXA *boxes = pixConnComp(pix, NULL, 4);
        BOX *box = boxaBoundingRegion(boxes);
        PIX *clipped = pixClipRectangle(gray, box, NULL);

        PIX *padded = pixCreate(box->w + (padLeft + padRight), box->h + (padBottom + padTop), 8);
        pixRasterop(padded, padLeft, padTop, padded->w, padded->h, PIX_SRC | PIX_DST, clipped, 0, 0);

        if (debug_pix) {
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

        pixDestroy(&clipped);
        pixDestroy(&gray);
        boxaDestroy(&boxes);
        boxDestroy(&box);

        return padded;
    }
    else if (pix->d == 8)
    {

    }
    else if (pix->d == 32)
    {
      /*
      // convert color(32) to grayscale
      PIX* pdata = pixConvertTo8(pix, 0);
      //   pdata = pixThreshold8(pdata, 8, 32, 0);
      pdata = reduce(pix, 400, 0);
      pdata = pixConvertTo8(pdata, 0);
      //    pdata = reduce(pdata, 500, 300);
      */
    }
    else
    {
        // Grayscale 8bpp/ Color 16/32 bpp
        // We just clean up border connected components
        int_t xres = pixGetXRes(pix);
        int_t yres = pixGetYRes(pix);
        PIX*  pix8 = pixConvertTo8(pix, 0);

        return pix8;
    }
}

PIX* normalize(PIX *pix)
{
    return normalize(pix, 0, 0, 0, 0);
}

PIX* reduce(PIX *pix, int width, int height)
{
    return pixScaleToSize(pix, width, height);
}
