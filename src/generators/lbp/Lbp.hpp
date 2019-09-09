#ifndef LBP_MATCHER_LBP_HPP
#define LBP_MATCHER_LBP_HPP

#include <LbpGenerator.hpp>

class LBP : LBPGenerator {

public:

    LBP();
    ~LBP();

    void generate(int** matrix, PIX* pix);
};

#endif //LBP_MATCHER_LBP_HPP
