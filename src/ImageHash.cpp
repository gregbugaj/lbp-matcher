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

///  statics
hash_t ImageHash::hash(PIX* pix, const HashMethod& method)
{
    auto handler = ImageHash::handlers[method];
    return handler->hash(pix);
}

hash_t ImageHash::hash(const std::string &filename, const ImageHash::HashMethod &method)
{
    validateFileExists(filename);
    PIX* pix = pixRead(filename.c_str());
    auto hs  = hash(pix, method);

    pixDestroy(&pix);

    return hs;
}

ImageHash::HandlerTypeMap ImageHash::createHandlers()
{
    HandlerTypeMap types;

    types.insert(std::make_pair(HashMethod::AVERAGE,    new AverageHash));
    types.insert(std::make_pair(HashMethod::DIFFERENCE, new DifferenceHash));
    types.insert(std::make_pair(HashMethod::PERCEPTUAL, new PerceptualHash));

    return types;
}