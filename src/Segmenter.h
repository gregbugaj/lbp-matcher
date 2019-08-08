#ifndef LBP_MATCHER_SEGMENTER_H
#define LBP_MATCHER_SEGMENTER_H

#include <vector>
#include <ostream>

/**
 * Document segmenter
 */
class Segmenter
{
public :

    struct Segment
    {
        int x;
        int y;
        int w;
        int h;

        int row;
        int col;

        // each segment can be linked ot additional child segments
        std::vector<Segment> segments;
    };

    friend std::ostream& operator << (std::ostream &os, const Segment &segment)
    {
        os << segment.row << ", " << segment.col << "[ " <<  segment.x << ", " << segment.y <<", " << segment.w << ", " << segment.h << " ]";
        return os;
    }

    /***
     * Segment document
     *
     * @param w image width
     * @param h image height
     * @param blockw block width
     * @param blockh block height
     * @return
     */
    std::vector<Segmenter::Segment> segment(int w, int h, int blockw, int blockh);
};


#endif //LBP_MATCHER_SEGMENTER_H
