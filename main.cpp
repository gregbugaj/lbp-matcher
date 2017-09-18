#include <iostream>
#include "HistogramComparison.h"
#include "AverageHash.h"


int main()
{
    ImageHash* imghash = new AverageHash();
    double h1 = imghash->hash(nullptr);
    
    std::cout<< "Average Hash = " << h1 << "\n";

    return 0;
}


int mainXX()
{
    LBPModel model;
    LBPModel sample;

    model.bins[0] = 1;
    model.bins[1] = 2;
    model.bins[2] = 3;
    model.bins[3] = 4;

    sample.bins[0] = 1;
    sample.bins[1] = 2;
    sample.bins[2] = 2;
    sample.bins[3] = 4;

    HistogramComparison histogram;

    auto r1 = histogram.compare(model, sample,  HistogramComparison::CompareType::INTERSECTION);
    auto r2 = histogram.compare(model, sample,  HistogramComparison::CompareType::CHI_SQUARED);
    auto r3 = histogram.compare(model, sample,  HistogramComparison::CompareType::LOG_LIKELIHOOD);
    auto r4 = histogram.compare(model, sample,  HistogramComparison::CompareType::KULLBACK_LEIBLER_DIVERGENCE);


    std::cout<< "Intersection      = " << r1 << "\n";
    std::cout<< "Chi Squared       = " << r2 << "\n";
    std::cout<< "LogLikehood       = " << r3 << "\n";
    std::cout<< "Kullback-Leibler  = " << r4 << "\n";

    return 0;
}