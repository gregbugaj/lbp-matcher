//
// Created by gbugaj on 9/13/2017.
//

#ifndef LBP_MATCHER_DIFFERENCEHASH_H
#define LBP_MATCHER_DIFFERENCEHASH_H


#include "ImageHash.h"

class DifferenceHash : public ImageHash
{
public :
    virtual double hash() const override;
};


#endif //LBP_MATCHER_DIFFERENCEHASH_H
