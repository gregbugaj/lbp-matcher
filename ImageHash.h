#ifndef LBP_MATCHER_IMAGEHASH_H
#define LBP_MATCHER_IMAGEHASH_H

#include "leptonica/allheaders.h"
#include "types.h"
#include <map>

class ImageHash
{

public :

    enum HashMethod
    {
        AVERAGE,
        DIFFERENCE,
        PERCEPTUAL
    };

    using HandlerTypeMap =  std::map<HashMethod, ImageHash*>;

    /**
     * Generate hash for given input image
     *
     * @param pix input image
     * @return  64 bit hash value
     */
    virtual hash_t hash(PIX* pix) const = 0;


    /**
     * Get hash value from image
     *
     * @param pix input image
     * @param method the method used to obtain hash
     * @return 64 bit hash value
     */
    static hash_t hash(PIX* pix, const HashMethod& method);


    /**
     * Get hash value from image provided via filename
     *
     * @param pix input image
     * @param method the method used to obtain hash
     * @return 64 bit hash value
     */
    static hash_t hash(const std::string& filename, const HashMethod& method);

    static HandlerTypeMap handlers;

protected:

    /**
     * Convert to correct format, this may involve up/down conversion.
     *
     * @param pix the pix to convert
     * @return converted pix
     */
    PIX* normalize(PIX *pix) const;

    /**
     * Reduce the size of given pix to given size
     * @param pix the pix to reduce
     * @param width  the width to reduce to
     * @param height the height to reduce to
     * @return reduced pix
     */
    PIX* reduce(PIX *pix, int width, int height) const;

private:

    static HandlerTypeMap createHandlers();

};

#endif //LBP_MATCHER_IMAGEHASH_H
