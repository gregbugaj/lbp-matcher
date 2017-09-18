#ifndef LBP_MATCHER_IMAGEHASH_H
#define LBP_MATCHER_IMAGEHASH_H

#include "leptonica/allheaders.h"
#include "types.h"

class ImageHash {

public :

    enum HashMethod
    {
        AVERAGE,
        DIFFERENCE,
        PERCEPTUAL
    };

    /**
     * Get hash value by given method
     * @param pix input image
     * @param method the method used to obtain hash
     * @return 64 bit hash value
     */
    static hash_t hash(PIX* pix, HashMethod& method);

    /**
     * Generate hash for given input image
     *
     * @param pix input image
     * @return  64 bit hash value
     */
    virtual hash_t hash(PIX* pix) const = 0;

    /**
     * Compare two different hashes and return their similarity in range 0..1
     * Default implementation uses "Hamming distance"
     *
     * @param hash1 the first hash to compare
     * @param hash2 the second hash to compare
     * @return similarity in range 0..1 0 = No similarity 1 = Full match
     */
    double_t similarity(hash_t& hash1, hash_t& hash2) const;

protected:
    /**
     * Convert to correct format, this may involve up/down conversion
     * @param pix the pix to convert
     * @return converted pix
     */
    PIX* convert(PIX *pix) const;


    /**
     * Reduce the size of given pix to given size
     * @param pix the pix to reduce
     * @param width  the width to reduce to
     * @param height the height to reduce to
     * @return reduced pix
     */
    PIX* reduce(PIX *pix, int width, int height) const;
};

#endif //LBP_MATCHER_IMAGEHASH_H
