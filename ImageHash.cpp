#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "ImageHash.h"
#include "AverageHash.h"
#include "DifferenceHash.h"
#include "PerceptualHash.h"
#include "fileutil.h"

ImageHash::HandlerTypeMap  ImageHash::handlers  = createHandlers();


PIX *ImageHash::normalize(PIX *pix) const
{
    if(pix->d != 8)
        return pixConvertTo8(pix, 0);
    else
        return pixClone(pix);
}

PIX *ImageHash::reduce(PIX *pix, int width, int height) const
{
    PIX* out = pixScaleToSize(pix, width, height);
    dump(out);
    return out;
}

///  statistics
hash_t ImageHash::hash(PIX *pix, const HashMethod& method)
{
    auto handler = ImageHash::handlers[method];
    return handler->hash(pix);
}


hash_t ImageHash::hash(const std::string &filename, const ImageHash::HashMethod &method)
{
    std::cout << "Reading file : " << filename;
    validateFileExists(filename);

    PIX* pix = pixRead(filename.c_str());
    return hash(pix, method);
}

ImageHash::HandlerTypeMap ImageHash::createHandlers()
{
    HandlerTypeMap types;

    types.insert(std::make_pair(HashMethod::AVERAGE,    new AverageHash));
    types.insert(std::make_pair(HashMethod::DIFFERENCE, new DifferenceHash));
    types.insert(std::make_pair(HashMethod::PERCEPTUAL, new PerceptualHash));

    return types;
}

