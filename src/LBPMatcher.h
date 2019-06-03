#ifndef LBP_MATCHER_LBPMATCHER_H
#define LBP_MATCHER_LBPMATCHER_H


#include <leptonica/allheaders.h>
#include "types.h"
#include "fileutil.h"


class LBPMatcher
{

public :

    /**
     * Crate Enhanced (improved) Local Binary Pattern image
     * Typical configurations :
     * P, R = (8, 1), (16, 2) and (8, 2)
     *
     * @param pix to create pattern for
     * @param points to use
     * @param radius to use
     * @return LBPModel
     */
    static LBPModel createLBPEnhanced(PIX* pix, int_t points, int_t radius);

    static LBPModel createLBPEnhanced(const std::string& filename, int_t points, int_t radius);

    /**
     * Crate Local Binary Pattern image
     *
     * @param pix to create pattern ofr
     * @return LBPModel
     */
    static LBPModel createLBP(PIX* pix);

    /***
     * Create LBP for given image
     *
     * @param filename to create model for
     * @return LBP Model representation of the image
     */
    static LBPModel createLBP(const std::string& filename);

private:

    static int sign(l_int32 point, l_int32 center);
};


#endif //LBP_MATCHER_LBPMATCHER_H
