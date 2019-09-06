#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "ImageHash.h"
#include "AverageHash.h"
#include "DifferenceHash.h"
#include "PerceptualHash.h"
#include "fileutil.h"
#include "HashDistance.h"

// Handlers used to create different hashes
ImageHash::HandlerTypeMap  ImageHash::handlers  = createHandlers();

///  statics
hash_t ImageHash::hash(PIX* pix, const HashMethod& method)
{
    auto handler = ImageHash::handlers[method];
    return handler->hash(pix);
}

double ImageHash::hash(PIX* pix1, PIX* pix2, const HashMethod& method)
{
    auto handler = ImageHash::handlers[method];
    auto h1 = handler->hash(pix1);
    auto h2 = handler->hash(pix2);

    HashDistance hs ;
    return hs.normalized(h1, h2);
}

hash_t ImageHash::hash(const std::string &filename, const ImageHash::HashMethod &method)
{
    validateFileExists(filename);
    PIX* pix = pixRead(filename.c_str());
    if(pix)
    {
        auto hs = hash(pix, method);
        pixDestroy(&pix);
        return hs;
    }

    return 0;
}

ImageHash::HandlerTypeMap ImageHash::createHandlers()
{
    HandlerTypeMap types;

    types.insert(std::make_pair(HashMethod::AVERAGE,    new AverageHash));
    types.insert(std::make_pair(HashMethod::DIFFERENCE, new DifferenceHash));
    types.insert(std::make_pair(HashMethod::PERCEPTUAL, new PerceptualHash));

    return types;
}