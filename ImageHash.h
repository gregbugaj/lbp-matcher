#ifndef LBP_MATCHER_IMAGEHASH_H
#define LBP_MATCHER_IMAGEHASH_H


class ImageHash {

public :

    enum HashMethod
    {
        AVERAGE,
        DIFFERENCE,
        PERCEPTUAL
    };

    virtual double hash() const = 0 ;
};

#endif //LBP_MATCHER_IMAGEHASH_H
