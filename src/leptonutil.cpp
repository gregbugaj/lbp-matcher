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
    PIX* pixt = pixCopy(NULL, pix);

    pixt = pixConvert1To8(NULL, pixt, 255, 0);
    pixt = pixScaleGray2xLI(pixt);
    pixt = pixScaleGray2xLI(pixt);
    pixt = pixBlockconv(pixt, 1, 1);
    pixt = pixBlockconv(pixt, 1, 1);
    pixt = pixBlockconv(pixt, 1, 1);
    pixt = pixScaleAreaMap2(pixt);
    pixt = pixScaleAreaMap2(pixt);

    return pixt;
}


PIX* normalize(PIX *pix, int padLeft, int padRight, int padTop, int padBottom)
{
    static int counter = 0;
    counter++;
    // Binary Image
    if(pix->d == 1)
    {
        PIX*  gray     = pixConvertTo8(pix, NULL); //create_grayscale(pix);
        //PIX*  gray     = create_grayscale(pix); -- CAUSES TOO MANY ARTIFACTS
        BOXA* boxes    = pixConnComp(pix, NULL, 4);
        BOX*  box      = boxaBoundingRegion(boxes);
        PIX*  cliped   = pixClipRectangle(gray, box, NULL);

        PIX* padded = pixCreate(box->w+(padLeft + padRight), box->h + (padBottom + padTop), 8);
        pixRasterop(padded, padLeft, padTop, padded->w, padded->h, PIX_SRC | PIX_DST, cliped, 0, 0);

        if(debug_pix)
        {

            char f1[255];
            sprintf(f1, "/home/gbugaj/share/devbox/tmp/gray-%d.png", counter);
            sprintf(f1, "/home/gbugaj/share/devbox/tmp/padded-%d.png", counter);
            sprintf(f1, "/home/gbugaj/share/devbox/tmp/cliped-%d.png", counter);

            pixWritePng(f1, gray, 0);
            pixWritePng(f1, padded, 0);
            pixWritePng(f1, cliped, 0);

        }

        pixDestroy(&cliped);
        pixDestroy(&gray);
        boxaDestroy(&boxes);
        boxDestroy(&box);

        return padded;
    }
    else
    {
        // Grayscale 8bpp/ Color 16/32 bpp
        // We just clean up border connected components
        int_t xres = pixGetXRes(pix);
        int_t yres = pixGetYRes(pix);
        PIX*  pix8 = pixConvertTo8(pix, 0);

        if(true)
            return pix8;

        PIX *binary;
        pixOtsuAdaptiveThreshold(pix8, (xres * 0.25 + 0.5), (yres * 0.25 + 0.5), 150, 150, 0.1, NULL, &binary);

        BOXA* boxes = pixConnComp(binary, NULL, 4);
        BOX*  box   = boxaBoundingRegion(boxes);
        PIX*  out   = pixClipRectangle(pix8, box, NULL);

        if (debug_pix)
        {
            pixWritePng("/tmp/lbp-matcher/out.png", out, 1);
            pixWritePng("/tmp/lbp-matcher/binary.png", binary, 1);
        }

        pixDestroy(&pix8);
        pixDestroy(&binary);
        boxaDestroy(&boxes);
        boxDestroy(&box);
        boxDestroy(&box);

        return out;
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
