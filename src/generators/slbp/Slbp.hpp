#ifndef LBP_MATCHER_SLBP_HPP
#define LBP_MATCHER_SLBP_HPP


#include <LbpGenerator.hpp>

/**
 * Signed Local Binary Pattern
 */
class SLBP : public LBPGenerator  {
public:
    SLBP();
    void generate(int** matrix, PIX* pix) const;
    ~SLBP();
};

#endif //LBP_MATCHER_SLBP_HPP
