#ifndef LBP_MATCHER_LEPTONUTIL_H
#define LBP_MATCHER_LEPTONUTIL_H

#include <leptonica/allheaders.h>
#include "types.h"

/**
 * Utility functions for working with Leptonica PIX data
 */

/**
 * Dump PIX data to console
 * This could take a while if the image is large
 *
 * @param pix the pix to dump
 */
void dump(PIX* pix);


/**
 * Create grayscale image
 *
 * @param pix the pix to convert to grayscale
 * @return
 */
PIX* create_grayscale(PIX* pix);

/**
 * Sum pixel values
 *
 * @param pix
 * @return
 */
l_uint32 sum(PIX* pix);

/**
 * Get minimum bounding box for given box set
 *
 * @param boxes
 * @return
 */
BOX* boxaBoundingRegion(BOXA* boxes);

/**
 * Get pixel value at specified location
 *
 * @param pix the non null pix image
 * @param x location
 * @param y location
 * @return pixel value
 */
l_uint32 pixAtGet(PIX* pix, int_t x, int_t y);

/**
 * Set value of a PIX at given location
 *
 * @param pix
 * @param x
 * @param y
 * @param value
 */
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
 * If the image is binary then the image will be converted to grayscale
 *
 * @param padLeft the amount of padding to add
 * @param padRight  the amount of padding to add
 * @param padTop  the amount of padding to add
 * @param padBottom  the amount of padding to add
 * @param pix the pix to convert
 * @return converted pix
 */
PIX* normalize(PIX *pix, int padLeft, int padRight, int padTop, int padBottom);

/**
 * Reduce the size of given pix to given size
 *
 * @param pix the pix to reduce
 * @param width  the width to reduce to
 * @param height the height to reduce to
 * @return reduced pix
 */
PIX* reduce(PIX *pix, int width, int height) ;

#endif //LBP_MATCHER_LEPTONUTIL_H
