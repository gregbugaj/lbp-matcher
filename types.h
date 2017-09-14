#ifndef LBP_MATCHER_TYPES_H
#define LBP_MATCHER_TYPES_H

typedef bool            bool_t;
typedef char            char_t;
typedef unsigned char   byte_t;
typedef int             int_t;
typedef long long       long_t;

struct LBPModel
{
    static const int_t bin_size = 256;
    int_t bins[bin_size] = {};
};

#endif //LBP_MATCHER_TYPES_H
