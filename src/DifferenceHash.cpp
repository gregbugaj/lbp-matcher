#include "DifferenceHash.h"
#include "leptonutil.h"



hash_t DifferenceHash::hash(PIX* pix) const
{
    static int counter = 0;

    int_t w = 9;
    int_t h = 8;

    PIX* normalized = normalize(pix);
    PIX* reduced    = reduce(normalized, w, h);

    hash_t out = 0;

    int_t wpl      = pixGetWpl(reduced);
    l_uint32* data = pixGetData(reduced);
    l_uint32* line;

    for (int_t y = 0; y < h; ++y)
    {
        line = data + y * wpl;
        for (int_t x = 0, ws = w - 1; x < ws; ++x)
        {
            int_t pos = y * w + x;
            l_uint32 current = GET_DATA_BYTE(line, x);
            l_uint32 next    = GET_DATA_BYTE(line, x+1);
            uinit64_t one = (current > next) ? 1 : 0;
            out |= one << pos;
            //trace(out, pos, one);
        }
    }

    pixDestroy(&normalized);
    pixDestroy(&reduced);

    return out;
}
