#include <iostream>
#include <bitset>
#include <climits>
#include <math.h>
#include "HashDistance.h"
#include "low.h"

constexpr size_t hashsize()
{
    return CHAR_BIT * sizeof(hash_t);
}

int HashDistance::distance(const hash_t &x, const hash_t &y) const
{
    auto z = x ^ y;
    auto p = popcnt(z);

    if(debug)
    {
        std::cout<<"---- HashDistance ---- " << std::endl;
        std::cout<<"size  : " << std::dec << hashsize() << std::endl;
        std::cout<<"x val : " << std::bitset<hashsize()>(x) << std::endl;
        std::cout<<"y val : " << std::bitset<hashsize()>(y) << std::endl;
        std::cout<<"z val : " << std::bitset<hashsize()>(z) << std::endl;
        std::cout<<"pop   : " << p << std::endl;
    }

    return p;
}

double HashDistance::normalized(const hash_t &x, const hash_t &y) const
{
    return normalize (distance(x, y));
}

double HashDistance::normalize(const int& distance) const
{
    auto size = static_cast<double>(hashsize());

    if(distance < 0 || distance > size)
        throw std::runtime_error(std::string("Distance out of bounds for  : ")
                                         .append(" hash = " + (int)size)
                                         .append(" distance = " + distance));

    return 1 - (round((distance / size) * 100.0) / 100.0);
}
