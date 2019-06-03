#ifndef LBP_MATCHER_DIFFERENCEHASH_H
#define LBP_MATCHER_DIFFERENCEHASH_H


#include "ImageHash.h"

class DifferenceHash : public ImageHash
{
public :
    hash_t hash(PIX* pix) const override;
};


#endif //LBP_MATCHER_DIFFERENCEHASH_H
