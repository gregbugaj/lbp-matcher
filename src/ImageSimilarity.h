#ifndef LBP_MATCHER_IMAGESIMILARITY_H
#define LBP_MATCHER_IMAGESIMILARITY_H


#include "types.h"
#include "ImageHash.h"

class ImageSimilarity
{
public:

    /**
     * Compare two different models and return their similarity in rang 0..1
     *
     * @param model the model we comparing against
     * @param target  the target that we are comparing
     * @return similarity in range 0..1 0 = No similarity 1 = Full match
     */
    static double_t similarity(PIX* model, PIX* target, const ImageHash::HashMethod &method);

    static double_t similarity(const std::string& srcFileName, const std::string& targetFileName, const ImageHash::HashMethod &method);
};


#endif //LBP_MATCHER_IMAGESIMILARITY_H
