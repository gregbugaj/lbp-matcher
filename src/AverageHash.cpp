#include <iostream>
#include "AverageHash.h"


hash_t AverageHash::hash(PIX* pix) const
{
    static int counter = 0;
    int_t w = 8;
    int_t h = 8;

    PIX* normalized = normalize(pix);
    PIX* reduced    = reduce(normalized, w, h);

    l_uint32 total  = sum(reduced);
    l_uint32 avg    = total / (w * h);

/*    char f[255];
    sprintf(f, "/tmp/lbp-matcher/normalized-%d.png",counter);
    pixWritePng(f, normalized, 1);
    sprintf(f, "/tmp/lbp-matcher/reduced-%d.png",counter);
    pixWritePng(f, reduced, 1);*/

    ++counter;
    hash_t out = 0;

    int_t wpl      = pixGetWpl(reduced);
    l_uint32* data = pixGetData(reduced);
    l_uint32* line;

    for (int_t y = 0; y < h; ++y)
    {
        line = data + y * wpl;
        for (int_t x = 0; x < w; ++x)
        {
            int_t pos = y * w + x;
            l_uint32 val = GET_DATA_BYTE(line, x);
            uinit64_t one = (val > avg) ? 1 : 0;
            out |= one << pos;

//            trace(out, pos, one);
        }
    }

    pixDestroy(&normalized);
    pixDestroy(&reduced);

    return out;
}
