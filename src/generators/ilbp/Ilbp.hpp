
#ifndef LBP_MATCHER_ILBP_HPP
#define LBP_MATCHER_ILBP_HPP

#include <LbpGenerator.hpp>
/**
 * Improved Local Binary Patterns
 */
class ILBP : public LBPGenerator  {
public:
    ILBP();
    void generate(int** matrix, PIX* pix) const;
    ~ILBP();
};

#endif //LBP_MATCHER_ILBP_HPP
