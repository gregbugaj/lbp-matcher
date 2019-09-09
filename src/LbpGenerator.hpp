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
    virtual void generate(int** matrix, PIX* pix);

    virtual ~LBPGenerator();
};

#endif //LBP_MATCHER_LBPGENERATOR_HPP
