#ifndef LBP_MATCHER_HEATMAP_H
#define LBP_MATCHER_HEATMAP_H

#include <leptonica/allheaders.h>
#include "types.h"


/**
 * Create default heat mat for the provided grayscale image
 *
 * @param pix the pix to generate heatmap for
 * @return pix containing the pix
 */
PIX* heatmap(PIX* pix);


#endif //LBP_MATCHER_HEATMAP_H
