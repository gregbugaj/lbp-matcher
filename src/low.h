#ifndef LBP_MATCHER_LOW_H
#define LBP_MATCHER_LOW_H

#include <iomanip>
#include <memory>
#include <sstream>
#include <iostream>
#include <bitset>
#include <limits.h>
#include "types.h"

/**
 * Count number of a transitions
 *
 * @param a
 * @return count of transitions
 */
int transitions(int a);

/**
 * Count number of a transitions
 *
 * @param a
 * @return count of transitions
 */
int transition_lbp(byte_t a);

/**
 * Perform population count using 'popcount' instruction
 *
 * @tparam T
 * @param val
 * @return
 */
template <class T>
int popcnt(const T& val) noexcept
{
    int bitcount;
    __asm__ ("popcnt %1, %1" : "=r" (bitcount) : "0" (val));
    return bitcount;
}

/**
 * Output value as a bit string
 * @tparam T
 * @param out
 */
template <class T>
void bitstr(const T& out) noexcept
{
    std::bitset<CHAR_BIT * sizeof(T)> bs(out);
    auto val =  static_cast<int>(out);
    std::cout << "0x"
              << std::setw(8) << std::hex << val << " : "
              << std::setw(8) << std::dec << val<< " :: " << bs << std::endl;
}

#endif //LBP_MATCHER_LOW_H
