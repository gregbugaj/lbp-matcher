#ifndef LBP_MATCHER_HEATMAP_H
#define LBP_MATCHER_HEATMAP_H

#include <leptonica/allheaders.h>
#include "types.h"

/***
 * Lower grayscale values are replaced by colors to the left of the scale while higher grayscale values are to the right of the scale
 * Color maps from : https://www.learnopencv.com/applycolormap-for-pseudocoloring-in-opencv-c-python/
 */


/**
 * Create default heat mat for the provided grayscale image
 *
 * @param pix the pix to generate heatmap for
 * @return pix containing the pix
 */
PIX* heatmap(PIX* pix);

/**
 * Utility function to generate lookup table
 * @param pix
 */
void heatmapGenerateLookupTable(PIX* pix);

#endif //LBP_MATCHER_HEATMAP_H
