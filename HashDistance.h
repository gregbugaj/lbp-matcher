#ifndef LBP_MATCHER_HASHDISTANCE_H
#define LBP_MATCHER_HASHDISTANCE_H

#include "types.h"

class HashDistance
{

public :

    /**
     * Calculate distance between two hashes
     * By default this method uses Hamming Distance
     *
     * @param x first hash to compare
     * @param y second hash to compare
     * @return distance between two hashes
     */
    int distance(const hash_t& x, const hash_t& y) const;
};


#endif //LBP_MATCHER_HASHDISTANCE_H
