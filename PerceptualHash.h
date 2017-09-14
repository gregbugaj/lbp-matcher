//
// Created by gbugaj on 9/13/2017.
//

#ifndef LBP_MATCHER_PERCEPTUALHASH_H
#define LBP_MATCHER_PERCEPTUALHASH_H


#include "ImageHash.h"

class PerceptualHash : public  ImageHash
{
    virtual double hash() const override;
};


#endif //LBP_MATCHER_PERCEPTUALHASH_H
