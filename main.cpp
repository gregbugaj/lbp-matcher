#include <iostream>
#include "AverageHash.h"

int main()
{
    AverageHash h;
    auto result = h.hash();
    std::cout<< "hash " << result;

    return 0;
}