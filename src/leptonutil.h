#ifndef LBP_MATCHER_LEPTONUTIL_H
#define LBP_MATCHER_LEPTONUTIL_H
#include <leptonica/allheaders.h>
#include "types.h"

/// Utillity functions for working with pix data

/**
 * Dump PIX data to console
 * @param pix the pix to dump
 */
void dump(PIX* pix);

/**
 * Sum pixel values
 * @param pix
 * @return
 */
l_uint32 sum(PIX* pix);

/**
 * Get minimum bouding box for given box set
 * @param boxes
 * @return
 */
BOX* boxaBoundingRegion(BOXA* boxes);

l_uint32 pixAtGet(PIX* pix, int_t x, int_t y);
void pixAtSet(PIX* pix, int_t x, int_t y, byte_t value);


/**
 * Convert to correct format, this may involve up/down conversion.
 *
 * @param pix the pix to convert
 * @return converted pix
 */
PIX* normalize(PIX *pix);


/**
 * Convert to correct format, this may involve up/down conversion.
 *
 * @param padLeft
 * @param padRight
 * @param padTop
 * @param padBottom
 * @param pix the pix to convert
 * @return converted pix
 */
PIX* normalize(PIX *pix, int padLeft, int padRight, int padTop, int padBottom);


/**
 * Reduce the size of given pix to given size
 * @param pix the pix to reduce
 * @param width  the width to reduce to
 * @param height the height to reduce to
 * @return reduced pix
 */
PIX* reduce(PIX *pix, int width, int height) ;


#endif //LBP_MATCHER_LEPTONUTIL_H
