#ifndef LBP_MATCHER_AVERAGEHASH_H
#define LBP_MATCHER_AVERAGEHASH_H

#include "ImageHash.h"
#include "leptonutil.h"

class AverageHash : public ImageHash
{

public :
    hash_t hash(PIX* pix) const override;
};

#endif //LBP_MATCHER_AVERAGEHASH_H
