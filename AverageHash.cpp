#include <iostream>
#include <bitset>
#include <climits>
#include "AverageHash.h"


hash_t AverageHash::hash(PIX* pix) const
{
    int w = 8;
    int h = 8;
    int s = w * h;

    PIX *normalized = normalize(pix);
    PIX *reduced    = reduce(normalized, w, h);

    l_uint32 total  = sum(reduced);
    l_uint32 avg    = total / s;

    std::cout << "Total pix value : " << total << " avg = " << avg << std::endl;

    pixWritePng("/tmp/lbp-matcher/normalized.png", normalized, 1);
    pixWritePng("/tmp/lbp-matcher/reduced.png", reduced, 1);

    hash_t out = 0;

    int_t wpl      = pixGetWpl(reduced);
    l_uint32* data = pixGetData(reduced);
    l_uint32* line;

    for (int_t y = 0; y < h; ++y)
    {
        line = data + y * wpl;
        for (int_t x = 0; x < w; ++x)
        {
            l_uint32 val = GET_DATA_BYTE(line, x);
            l_uint32 zeroone = (val > avg) ? 1 : 0;
            out = out << 1 | zeroone;

            std::bitset<CHAR_BIT * sizeof(hash_t)> bs(out);
            std::cout<<"\n hash : " << std::hex << " 0x" << out << "  : " << zeroone  << " :: " << bs << std::endl;
        }
    }

    std::cout<<"\nHash hex : " << std::hex << " 0x" << out << std::endl;
    return out;
}
