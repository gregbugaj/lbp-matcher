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

enum MicroTexton
{
    SPOT      = 0x00, // 0000 0000
    SPOT_FLAT = 0xFF, // 1111 1111
    LINE_END  = 0x30, // 0011 0000
    EDGE      = 0xE1, // 1110 0001
    CORNER    = 0x70  // 0111 0000
};

class LBPMatcher
{

public :

    /**
     * Create a LBP model and populate the lbp matrix
     *
     * @param matrix
     * @param type
     * @param pix
     */
    static void createLBP(int** matrix, LbpType type, PIX* pix);

    /**
     * Extracted uniform local binary pattern (LBP) from a grayscale image.
     * The LBP features encode local texture information.
     *
     * @param pix to extract features form
     * @return LBPModel of extracted features
     */
    static LBPModel createLBP(PIX* pix);

    /***
     * Extracted uniform local binary pattern (LBP) from a grayscale image.
     * The LBP features encode local texture information.
     * *
     * @param filename to create model for
     * @return LBPModel of extracted features
     */
    static LBPModel createLBP(const std::string& filename);

private:

};


#endif //LBP_MATCHER_LBPMATCHER_H
