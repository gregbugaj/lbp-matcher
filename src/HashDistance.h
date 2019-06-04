#ifndef LBP_MATCHER_HASHDISTANCE_H
#define LBP_MATCHER_HASHDISTANCE_H

#include "types.h"

class HashDistance
{

public :

    bool debug = false;

    /**
     * Calculate distance between two hashes
     * By default this method uses Hamming Distance
     *
     * @param x first hash to compare
     * @param y second hash to compare
     * @return distance between two hashes
     */
    int distance(const hash_t& x, const hash_t& y) const;

    /**
     * Calculate distance between two hashes in range 0..1
     * 1 = Full match  0 = No Match
     *
     * @param x first hash to compare
     * @param y second hash to compare
     * @return normalized distance between two hashes
     */
    double normalized(const hash_t& x, const hash_t& y) const;

    /**
     * Normalize distance in range 0..1
     *
     * @param distance to normalize
     * @return  normalized distance
     */
    double normalize(const int& distance) const;
};


#endif //LBP_MATCHER_HASHDISTANCE_H
