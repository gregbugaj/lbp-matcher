#include <iostream>
#include "HistogramComparison.h"
#include "ImageHash.h"
#include "HashDistance.h"
#include "ImageSimilarity.h"


int mainXX333()
{
    /**
     bf3d45d5d505fdff
    ff3d5555d50505ff
    bf7d45d5d505ffff
    bf7d45d5d505ffff
    bf7d45d5d505fdff
    ff3d45d5d505fdff

     */
    HashDistance hs ;
    auto val = hs.distance(0x5013f7ffeccfbf1b, 0x5110abbbfdcc8ffb);
    std::cout<< "Average Hash = " << val << "\n";

    return 0;
}


int main()
{
    auto h1 = ImageHash::hash("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/41.png", ImageHash::AVERAGE);
    auto h2 = ImageHash::hash("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/42.png", ImageHash::AVERAGE);

    std::cout<< "Hashing result = " << h1 << "\n";
    std::cout<< "Hashing result = " << h2 << "\n";


    ImageSimilarity::similarity("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/42.png", "/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/42.png", ImageHash::AVERAGE);

/*    HashDistance hs ;
    auto val = hs.distance(0xfd3f9ad8e6eef884, 0xff3d1f8151465edd);
    std::cout<< "Average Hash = " << val << "\n";*/

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