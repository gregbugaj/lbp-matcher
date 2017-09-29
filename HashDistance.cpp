#include <iostream>
#include <bitset>
#include <climits>
#include "HashDistance.h"

int popcount64(const hash_t& val) noexcept
{
    int ret;
    __asm__ ("popcnt %1, %1" : "=r" (ret) : "0" (val));
    return ret;
}

int hamming_distance(const hash_t& x, const hash_t& y)
{
    auto z = x ^ y;
    auto p = popcount64(z);
    auto bitsize = CHAR_BIT * sizeof(unsigned long long);

    std::cout<< std::dec;
    std::cout<<"size cb  : " << CHAR_BIT << std::endl;
    std::cout<<"size so : " << sizeof(unsigned long long) << std::endl;
    std::cout<<"size so : "<< std::hex << bitsize<< std::endl;
    std::cout<<"size co : " << static_cast<int64_t>(bitsize) << std::endl;
    printf("size pf : %d \n", bitsize);

    std::cout<<"x val : " << std::bitset<CHAR_BIT * sizeof(hash_t)>(x) << std::endl;
    std::cout<<"y val : " << std::bitset<CHAR_BIT * sizeof(hash_t)>(y) << std::endl;
    std::cout<<"z val : " << std::bitset<CHAR_BIT * sizeof(hash_t)>(z) << std::endl;
    std::cout<<"pop   : " << p << std::endl;

    return p;
}

int HashDistance::distance(const hash_t &x, const hash_t &y) const
{
    return hamming_distance(x, y);
}
