#ifndef LBP_MATCHER_AVERAGEHASH_H
#define LBP_MATCHER_AVERAGEHASH_H

#include "ImageHash.h"

class AverageHash : public ImageHash {

public :
    virtual hash_t hash(PIX* pix) const override;

};

#endif //LBP_MATCHER_AVERAGEHASH_H
