#ifndef LBP_MATCHER_LEPTONUTIL_H
#define LBP_MATCHER_LEPTONUTIL_H
#include <leptonica/allheaders.h>
#include "types.h"

/// Utillity functions for working wiht pix data

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

#endif //LBP_MATCHER_LEPTONUTIL_H
