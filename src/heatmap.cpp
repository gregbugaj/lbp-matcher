#include "heatmap.h"
#include "types.h"
#include "leptonutil.h"
/**
 * ref http://tpgit.github.io/Leptonica/colormap_8c_source.html#l00632
 * https://github.com/DanBloomberg/leptonica/blob/master/src/coloring.c
 */

PIX *heatmap(PIX *pix)
{
    int w = pix->w;
    int h = pix->h;
    PIXCMAP* cmap = pixcmapRead("/tmp/lbp-matcher/cmap.txt");
    RGBA_QUAD* cta = (RGBA_QUAD *)cmap->array;
    l_int32    rval, gval, bval;
    PIX* dpix = pixCreate(w, h, 8);

    l_int32 wpl = pixGetWpl(dpix);
    l_uint32 *data = pixGetData(dpix);
    l_uint32* line;

    for (int_t y = 0; y < h; ++y)
    {
        line = data + y * wpl;
        for (int_t x = 0; x < w; ++x)
        {
  /*          l_uint32 pixel = GET_DATA_BYTE(line, x);
            rval = cta[pixel].red;
            gval = cta[pixel].green;
            bval = cta[pixel].blue;*/

//            SET_DATA_BYTE(line, x, pixel);//

            l_uint32 val;
            pixGetPixel(pix, x, y, &val);
//            pixSetPixel(dpix, x, y , val);
            SET_DATA_BYTE(line, x, val);//
            //pixSetRGBPixel(dpix, x, y, rval, gval, bval);
        }
    }

    pixWritePng("/tmp/lbp-matcher/heat.png", dpix, 0);
    pixDestroy(&dpix);
}

void heatmapGenerateLookupTable(PIX* pix) {
    std::cout << "Color map : " << pix->d << std::endl;
    int w = pix->w;
    int h = pix->h;
    int d = pix->d;

    std::cout << "Color map : " << pix->d << " colormap size WxH :" << w << "," << h << std::endl;
    if (w != 256)
        throw std::runtime_error("Colormap has to be 256 wide");

    // we only need one row for sampling
    int y = 0;
    l_int32 wpl = pixGetWpl(pix);
    l_uint32 *data = pixGetData(pix);
    l_uint32 *line = data + y * wpl;

    PIXCMAP* cmap = pixGetColormap(pix);
    RGBA_QUAD* cta = (RGBA_QUAD *)cmap->array;
    l_int32    rval, gval, bval;

    pixcmapWrite ("/tmp/lbp-matcher/cmap.txt", cmap);

    if(d == 8)
    {
        for (int j = 0; j < w; ++j)
        {
            l_uint32 pixel = GET_DATA_BYTE(line, j);
            rval = cta[pixel].red;
            gval = cta[pixel].green;
            bval = cta[pixel].blue;
            std::cout << j << " : " << pixel << " =  " << rval << ", " << gval << ", " << bval << std::endl;
        }
    }
    else /* d == 32 */
    {
            for (int j = 0; j < w; ++j)
            {
                l_uint32 pixel = line[j];
                extractRGBValues(pixel, &rval, &gval, &bval);
                std::cout << j << " : " << pixel << " =  " << rval << ", " << gval << ", " << bval << std::endl;
            }
    }
}