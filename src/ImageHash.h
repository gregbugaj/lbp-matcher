#ifndef LBP_MATCHER_IMAGEHASH_H
#define LBP_MATCHER_IMAGEHASH_H

#include "leptonica/allheaders.h"
#include "types.h"
#include <map>
#include <bitset>
#include <climits>
#include <iomanip>
#include <iostream>

class ImageHash
{

public :

    bool debug = true;

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

    /***
     * Debug trace
     * @param out
     * @param pos
     * @param one
     */
    void trace(hash_t out, int_t pos, uinit64_t one) const
    {
        if(debug)
        {
            std::bitset<CHAR_BIT * sizeof(hash_t)> bs(out);
            std::cout << "hash : pos [" << std::dec << std::setw(2) << pos << "] " << std::hex << " 0x"
                      << std::setw(16) << out << " : " << one << " :: " << bs << std::endl;
        }
    }


private:

    static HandlerTypeMap createHandlers();

};

#endif //LBP_MATCHER_IMAGEHASH_H
