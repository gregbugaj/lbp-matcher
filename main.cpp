#include <iostream>
#include "HistogramComparison.h"

int main()
{
    LBPModel model;
    LBPModel sample;

    model.bins[0]  = 1;
    sample.bins[0] = 2;

    HistogramComparison histogram;
    auto result = histogram.compare(model, sample,  HistogramComparison::CompareType::INTERSECTION);

    std::cout<< "hash " << result;

    return 0;
}