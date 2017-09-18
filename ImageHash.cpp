#include "ImageHash.h"


double_t ImageHash::similarity(hash_t &hash1, hash_t &hash2) const
{
    return 0;
}


// statics

hash_t ImageHash::hash(PIX *pix, ImageHash::HashMethod& method)
{
    switch(method)
    {
        case AVERAGE :   return 0;
        case DIFFERENCE : return 0;
        case PERCEPTUAL : return 0;
    }

    return 0;
}


PIX *ImageHash::convert(PIX *pix) const
{

    return nullptr;
}

PIX *ImageHash::reduce(PIX *pix, int width, int height) const
{

    return nullptr;
}
