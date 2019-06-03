#ifndef LBP_MATCHER_PERCEPTUALHASH_H
#define LBP_MATCHER_PERCEPTUALHASH_H


#include "ImageHash.h"

class PerceptualHash : public  ImageHash
{
    virtual hash_t hash(PIX* pix) const override;
};


#endif //LBP_MATCHER_PERCEPTUALHASH_H
