#ifndef LBP_MATCHER_LBPGENERATOR_HPP
#define LBP_MATCHER_LBPGENERATOR_HPP

#include "leptonutil.h"

class LBPGenerator {

public:
    /**
     * Crate Local Binary Pattern image
     *
     * @param matrix the lbp matrix to populate
     * @param pix the pix to generate lbp for
     */
    virtual void generate(int** matrix, PIX* pix) const = 0;

    /**
     * default destructor
     */
    virtual ~LBPGenerator() = default;

    inline l_int32 pixAtGetSan(int** data, int_t h, int_t w, int_t x, int_t y) const
    {
        if(y < 0 || y >= h || x < 0 || x >= w)
            return 0;
        return  data[y][x];
    }

    /**
    * Get PIX value at specific location while performing bounds check
    *
    * @param pix
    * @param x
    * @param y
    * @return
    */
    l_int32  pixAtGetSan(PIX* pix, int_t x, int_t y) const
    {
        if(y < 0 || y >= pix->h || x < 0 || x >= pix->w)
            return 0;

        l_int32 wpl    = pixGetWpl(pix);
        l_uint32* data = pixGetData(pix);
        l_uint32* line = data + y * wpl;
        l_uint32 val = 0;

        if(pix->d == 1)
            val = GET_DATA_BIT(line, x);
        else if(pix->d == 2)
            val = GET_DATA_DIBIT(line, x);
        else if(pix->d == 4)
            val = GET_DATA_QBIT(line, x);
        else // 8, 16, 32
            val = GET_DATA_BYTE(line, x);

        return val;
    }
};

#endif //LBP_MATCHER_LBPGENERATOR_HPP
