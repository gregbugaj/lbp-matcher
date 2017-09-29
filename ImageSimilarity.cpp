#include <fstream>
#include <sstream>
#include "ImageSimilarity.h"
#include "fileutil.h"

double_t ImageSimilarity::similarity(PIX *model, PIX *target, const ImageHash::HashMethod &method)
{

    return 0;
}


double_t ImageSimilarity::similarity(const std::string &srcFileName, const std::string &targetFileName, const ImageHash::HashMethod &method)
{
    validateFileExists(srcFileName);
    validateFileExists(targetFileName);

    PIX* pix1 = pixRead(srcFileName.c_str());
    PIX* pix2 = pixRead(targetFileName.c_str());

    return similarity(pix1, pix2, method);
}
