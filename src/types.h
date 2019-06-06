#ifndef LBP_MATCHER_TYPES_H
#define LBP_MATCHER_TYPES_H

#include <iostream>
#include <memory>
#include <sstream>
#include <leptonica/allheaders.h>
#include <vector>

typedef double               double_t;
typedef bool                 bool_t;
typedef char                 char_t;
typedef unsigned char        byte_t;
typedef int                  int_t;
typedef unsigned long int    uinit64_t; // 8 bytes(64 bits)
typedef uinit64_t            hash_t;

struct LBPModel {
public:
    LBPModel(int msize) : size(msize) {
        bins.resize(size);

        for (int i = 0, s = msize; i < s; ++i) {
            bins[i] = 0;
        }
    }

    ~LBPModel() {
        bins.clear();
    }

    void append(const LBPModel &model)
    {
        std::cout << " append  "  << model << std::endl;
        int start = bins.size();
        bins.resize(bins.size() + model.size);

        for (int i = 0, s =  model.size; i < s; ++i) {
            bins[start + i] = model.bins[i];
        }
    }

    std::vector<int> bins; // size of the vector starts at 0
    int size;

    /**
     * Output histogram
     *
     * @param os
     * @param model
     * @return
     */
    friend std::ostream& operator << (std::ostream &os, const LBPModel &model)
    {
        std::string delim =  ", ";
        os << "Size (" << model.size << ") [";

        for(int i = 0, s = model.size; i < s; ++i)
        {
            if(i == s - 1)
                delim = "";
           os << model.bins[i] << delim;
        }
        os << "]";

        return os;
    }
};

template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_deleter : public std::shared_ptr<T>
{
    shared_ptr_with_deleter (T* t = nullptr)
    : std::shared_ptr<T> (t, D())
    {

    }

    void reset(T* t = nullptr)
    {
        printf("\nDELETING reset\n");
        std::shared_ptr<T>::reset(t, D());
    }

    /// allow for converting from SharedPtr to T*
    /// <pre>
    ///    PIXPtr scaled = pixScaleToSize(pix, w, h);
    ///    PIXPtr converted = pixConvertTo8(scaled, 0);
    /// </pre>
 /*
     operator T*() const
    {
        return std::shared_ptr<T>::get();
    }
  */
};

struct LEP_Deleter
{
    void operator() (PIX* ptr)
    {
        printf("\nDELETING LEP_Deleter\n");
        if(ptr) pixDestroy(&ptr);
    }
};

using PIXPtr = std::unique_ptr<PIX, LEP_Deleter>;

//using PIXShPtr = shared_ptr_with_deleter<PIX, LEP_Deleter>;

#endif //LBP_MATCHER_TYPES_H
