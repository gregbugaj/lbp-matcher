#ifndef LBP_MATCHER_TYPES_H
#define LBP_MATCHER_TYPES_H

#include <memory>
#include <leptonica/allheaders.h>

typedef double          double_t;
typedef bool            bool_t;
typedef char            char_t;
typedef unsigned char   byte_t;
typedef int             int_t;
typedef long long       hash_t; // 8 bytes(64)

struct LBPModel
{
    static const int_t bin_size = 256;
    int_t bins[bin_size] = {};
};

template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_deleter : public std::shared_ptr<T>
{
    explicit shared_ptr_with_deleter (T* t = nullptr)
    : std::shared_ptr<T> (t, D())
    {

    }

    void reset(T* t = nullptr)
    {
        std::shared_ptr<T>::reset(t, D());
    }
};

struct LEP_Deleter
{
    void operator() (PIX* ptr) {if(ptr) pixDestroy(&ptr);}
};

using PIXPtr = std::unique_ptr<PIX, LEP_Deleter>;

// using PIXShPtr = shared_ptr_with_deleter<PIX, LEP_Deleter>;

#endif //LBP_MATCHER_TYPES_H
