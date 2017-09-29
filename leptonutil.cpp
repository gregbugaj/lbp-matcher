#include <sstream>
#include "leptonutil.h"


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
        std::stringstream ss;
        ss << "PIX should be 8bpp, got : " << pix->d;

        throw std::runtime_error(ss.str());
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
            l_uint32 v = GET_DATA_BYTE(line, x);
            sum += v;
        }
    }

    return sum;
}