#ifndef LBP_MATCHER_LOW_H
#define LBP_MATCHER_LOW_H

#include <iomanip>
#include <memory>
#include <sstream>
#include <iostream>
#include <bitset>
#include <limits.h>
#include "types.h"


int transitions(int a);

int transition_lbp(byte_t a);

template <class T>
int popcnt(const T& val) noexcept
{
    int bitcount;
    __asm__ ("popcnt %1, %1" : "=r" (bitcount) : "0" (val));
    return bitcount;
}

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
