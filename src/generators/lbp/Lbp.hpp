#ifndef LBP_MATCHER_LBP_HPP
#define LBP_MATCHER_LBP_HPP

#include <LbpGenerator.hpp>

/**
 * Original Local Binary Pattern
 */
class LBP : public LBPGenerator {
public:
    LBP();
    void generate(int** matrix, PIX* pix) const;
    ~LBP();
};

#endif //LBP_MATCHER_LBP_HPP
