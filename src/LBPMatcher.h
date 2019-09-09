#ifndef LBP_MATCHER_LBPMATCHER_H
#define LBP_MATCHER_LBPMATCHER_H

#include <leptonica/allheaders.h>
#include <experimental/filesystem>
#include "types.h"
#include "fileutil.h"
#include "LbpGenerator.hpp"

namespace  fs = std::experimental::filesystem;

enum LbpType
{
    NORMAL,
    ENHANCED,
    SIGNED
};

class LBPMatcher
{

public :

    /**
     * Create a LBP model and populate the lbp matrix
     * Input image will be padded by each method individually to account for edges
     * Padding will be performed with value of 255 (because we are dealing with paper)
     *
     * @param matrix the lbp matrix to populate
     * @param type the type of LBP to use
     * @param pix the pix to create the LBP for
     */
    static void createLBP(int** matrix, PIX* pix, const LBPGenerator& generator);

    static int** createLBP(PIX* pix, const LBPGenerator& generator);

    /**
     * Create a LBP model and populate the lbp matrix
     * Input image will be padded by each method individually to account for edges
     * Padding will be performed with value of 255 (because we are dealing with paper)
     *
     * @param matrix the lbp matrix to populate
     * @param type the type of LBP to use
     * @param pix the pix to create the LBP for
     */
    static void createLBP(int** matrix, LbpType type, PIX* pix);

    /**
     * Create Extracted uniform local binary pattern (LBP) from a grayscale image.
     * The LBP features encode local texture information.
     *
     * @param pix to extract features from, expecting 8bpp image
     * @return Histogram of extracted features with default partitioning
     */
    static Histogram createLBP(PIX* pix);

    /***
     * Extracted uniform local binary pattern (LBP) from a grayscale image.
     * The LBP features encode local texture information.
     * *
     * @param filename to create model for
     * @return Histogram of extracted features
     */
    static Histogram createLBP(const std::string& filename);

     /**
      * Generate histogram for given lbp matrix
      * @param lbpMatrix
      * @param cols
      * @param rows
      * @param _x
      * @param _y
      * @param w
      * @param h
      * @return
      */
    static Histogram createLBPHistogram(int **lbpMatrix, l_int32 cols, l_int32 rows, l_int32 _x, l_int32 _y, l_int32 w, l_int32 h);

    static std::vector<int> UniformCodes;;
};


#endif //LBP_MATCHER_LBPMATCHER_H
