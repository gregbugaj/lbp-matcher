#ifndef LBP_MATCHER_LBPMATCHER_H
#define LBP_MATCHER_LBPMATCHER_H

#include <leptonica/allheaders.h>
#include <experimental/filesystem>
#include "types.h"
#include "fileutil.h"

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
     * @param matrix the matrix to populate
     * @param type the type of LBP to use
     * @param pix the pix to create the LBP for
     */
    static void createLBP(int** matrix, LbpType type, PIX* pix);

    /**
     * Dump the LBP matrix
     *
     * @param matrix the lbp matrix to dump
     * @param row the height of the matrix
     * @param cols the width of the matrix
     * @param filename the filename to output pix data to
     */
    static void pixFromMatrix(int **matrix, int rows, int cols, char* filename);

    /**
     * Dump the LBP matrix
     *
     * @param matrix the lbp matrix to dump
     * @param row the height of the matrix
     * @param cols the width of the matrix
     * @param filename the filename to output pix data to
     */
    static PIX* pixFromMatrix(int **matrix, int rows, int cols);

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
      * @param x
      * @param y
      * @param w
      * @param h
      * @return
      */
    static Histogram createLBPHistogram(int **lbpMatrix, l_int32 cols, l_int32 rows, l_int32 x, l_int32 y, l_int32 w, l_int32 h);

    static std::vector<int> UniformCodes;;
};


#endif //LBP_MATCHER_LBPMATCHER_H
