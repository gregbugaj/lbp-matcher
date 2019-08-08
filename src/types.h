#ifndef LBP_MATCHER_TYPES_H
#define LBP_MATCHER_TYPES_H

#include <iostream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <leptonica/allheaders.h>

typedef double               double_t;
typedef bool                 bool_t;
typedef char                 char_t;
typedef unsigned char        byte_t;
typedef int                  int_t;
typedef unsigned long int    uinit64_t; // 8 bytes(64 bits)
typedef uinit64_t            hash_t;

/**
 * round value to specific precision
 *
 * @param val the value to round
 * @param precision the precision to round to
 * @return
 */
template <class T>
double round(T val, int precision)
{
    std::stringstream s;
    s << std::setprecision(precision) << std::setiosflags(std::ios_base::fixed) << val;
    s >> val;
    return val;
}

/**
 * Check if two values are equal
 *
 * Ref
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 * https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
 * http://realtimecollisiondetection.net/blog/?p=89
 * https://stackoverflow.com/questions/41213480/comparing-doubles-properly-using-aliasing-with-integer-representations-and-ulps
 *
 * @tparam T the type of the parameter
 * @param a
 * @param b
 * @param epsilon
 * @return true if the value is within epsilon value
 */

inline bool equal(double a, double b, double epsilon)
{
    // assume small positive epsilon
    if(!(epsilon >= 0.0f && epsilon <= 1.0f))
        throw std::runtime_error("epislon value out of range 0.0 .. 1.0  got : " + std::to_string(epsilon));
    // for relative equality
    // |a - b| < = epsilon * max(|a|, |b|)
    //auto epsilon = std::numeric_limits<double>::epsilon();
    auto val = std::abs(a -  b) <= epsilon;
    return val;
}

struct Histogram {
public:
    Histogram(int size)
    {
        normalized = false;
        bins.resize(size, 0);
    }

    ~Histogram() {
        bins.clear();
    }

    /**
     * Overloading [] operator to access elements in array style
     * This function must return a reference as array element can be put on left side
     * example :
     * <code>
     * model[i] = 0
     * int val = model[i]
     * </code>
     *
     * @param index
     * @return
     */
    double& operator[](int index)
    {
        checkIndex(index);
        return bins[index];
    }

    double operator[](int index) const
    {
        checkIndex(index);
        return bins[index];
    }

    /**
     * Get size of histogram
     *
     * @return size of histogram
     */
    unsigned long size() const
    {
        return bins.size();
    }

    /**
     * Append model to current model
     *
     * @param model the model to append
     */
    void append(const Histogram &model)
    {
        bins.insert(std::end(bins), std::begin(model.bins), std::end(model.bins));
    }

    /**
     * Output histogram
     *
     * @param os
     * @param model
     * @return
     */
    friend std::ostream& operator << (std::ostream &os, const Histogram &model)
    {
        std::string delim =  ", ";
        os << "Size (" << model.size() << ") [";

        for(int i = 0, s = model.size(); i < s; ++i)
        {
            if(i == s - 1)
                delim = "";
           os << model[i] << delim;
        }
        os << "]";

        return os;
    }

    /**
     * Get underlying vector
     *
     * @return
     */
    std::vector<double> getVector() const {
        return bins;
    }

    /**
     * Check if histogram is normalized
     * @return
     */
    bool isNormalized() const {
        return normalized;
    }
    /**
     * The normalized count is the count in a class divided by the total number of observations.
     * In this case the relative counts are normalized to sum to one
     */
    void normalize()
    {
        if(normalized)
            throw std::runtime_error("Histogram already normalized");

        auto sum = .0;
        auto total = .0;
        auto s = bins.size();

        for(int i = 0; i < s; ++i)
            sum += bins[i];

        if(sum == 0.0)
        {
            normalized = true;
            return;
        }

        for(int i = 0; i < s; ++i)
        {
            bins[i] = bins[i] / sum;
            total += bins[i];
        }

        if(!equal(1.0, total, 0.100000))
            throw std::runtime_error("Normalized total not equal to 1.0(+-)epsilon got : " + std::to_string(total));

        normalized = true;
    }

private :

    bool normalized;

    std::vector<double> bins;

    void checkIndex(int index) const {
        if(index < 0 || index > bins.size())
            throw std::runtime_error("Array index out of bound : " + std::to_string(index));
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
