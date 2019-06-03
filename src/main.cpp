#include <iostream>
#include "HistogramComparison.h"
#include "ImageHash.h"
#include "HashDistance.h"
#include "ImageSimilarity.h"
#include "LBPMatcher.h"
#include "low.h"
#include "Segmenter.h"
#include "Extractor.h"

#include <fstream>
#include <experimental/filesystem>
#include <chrono>
#include <set>

using namespace std::chrono;
namespace  fs = std::experimental::filesystem;

int mainAAX()
{
    auto m0 = LBPMatcher::createLBP("/home/gbugaj/share/devbox/data1/test.tif");
/*


    Extractor extractor;

    std::string document = "/home/gbugaj/share/devbox/data1/claim001.tif";
    std::string snip     = "/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/27.png";

    extractor.extract(document, snip);

 */
    return 0;
}


int mainafdsfdsf()
{
    Segmenter seg;
    auto segments = seg.segment(120, 120, 10, 10);

    for(auto& segment: segments)
    {
        std::cout << segment << "\n";
    }

//    std::cout<< "Hashing result = " << h2 << "\n";

    /*
    std::cout<< "A = " << hs.normalize(0) << "\n";
    std::cout<< "B = " << hs.normalize(32) << "\n";
    std::cout<< "C = " << hs.normalize(64) << "\n";
    std::cout<< "C = " << hs.normalize(26) << "\n";
    */
    return 0;
}


int maina()
{
    HistogramComparison comp;
    //auto m0 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/27.png");

    auto m0   = LBPMatcher::createLBP("/home/gbugaj/share/devbox/data1/face004.tif");
    //auto m0 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/8.png");
    //auto m0   = LBPMatcher::createLBP("/home/gbugaj/share/devbox/data1/anna-300x300.jpg");

    auto type = HistogramComparison::CompareType::INTERSECTION;
    auto s0 = comp.compare(m0, m0, type);

    std::cout << "\nScores :" << std::endl;
    std::cout << "s0 :" << std::dec << s0  << std::endl;

    return 0;
}

int mainxa()
{
    HistogramComparison comp;

    auto m0 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/8.png");
    auto m1 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/9.png");

    auto type = HistogramComparison::CompareType::INTERSECTION;

    auto s0 = comp.compare(m0, m0, type);
    auto s1 = comp.compare(m1, m1, type);
    auto s2 = comp.compare(m0, m1, type);
    auto s3 = comp.compare(m1, m0, type);

    std::cout << "\nScores :" << std::endl;
    std::cout << "s0 :" << std::dec << s0  << std::endl;
    std::cout << "s1 :" << std::dec << s1  << std::endl;
    std::cout << "s2 :" << std::dec << s2  << std::endl;
    std::cout << "s3 :" << std::dec << s3  << std::endl;


    std::cout << "Histograms " << std::endl;
    std::cout << m0 << std::endl;
    std::cout << m1<< std::endl;

    return 0;
}

int main3()
{
    HistogramComparison comp;

    //auto m0 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/27.png");
    //auto m1 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/28.png");

    auto m0 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-processed/0.png");
    auto m1 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-processed/1.png");
    auto m2 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-processed/2.png");

    auto type = HistogramComparison::CompareType::INTERSECTION;

    auto s0 = comp.compare(m0, m0, type);
    auto s1 = comp.compare(m0, m1, type);
    auto s2 = comp.compare(m0, m2, type);

    std::cout << "\nScores :" << std::endl;
    std::cout << "s0 :" << std::dec << s0  << std::endl;
    std::cout << "s1 :" << std::dec << s1  << std::endl;
    std::cout << "s2 :" << std::dec << s2  << std::endl;

    return 0;
}

int mainxxxa()
{
    HistogramComparison comp;

    auto m = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/9.png");

    auto m0 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/0.png");
    auto m1 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/1.png");
    auto m2 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/2.png");
    auto m3 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/3.png");
    auto m4 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/4.png");
    auto m5 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/5.png");
    auto m6 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/6.png");
    auto m7 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/7.png");
    auto m8 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/8.png");
    auto m9 = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/template-claim/9.png");

    auto type = HistogramComparison::CompareType::INTERSECTION;

    auto s0 = comp.compare(m, m0, type);
    auto s1 = comp.compare(m, m1, type);
    auto s2 = comp.compare(m, m2, type);
    auto s3 = comp.compare(m, m3, type);
    auto s4 = comp.compare(m, m4, type);
    auto s5 = comp.compare(m, m5, type);
    auto s6 = comp.compare(m, m6, type);
    auto s7 = comp.compare(m, m7, type);
    auto s8 = comp.compare(m, m8, type);
    auto s9 = comp.compare(m, m9, type);

    std::cout << "Scores :" << std::endl;
    std::cout << std::dec << s0  << std::endl;
    std::cout << std::dec << s1  << std::endl;
    std::cout << std::dec << s2  << std::endl;
    std::cout << std::dec << s3  << std::endl;
    std::cout << std::dec << s4  << std::endl;
    std::cout << std::dec << s5  << std::endl;
    std::cout << std::dec << s6  << std::endl;
    std::cout << std::dec << s7  << std::endl;
    std::cout << std::dec << s8  << std::endl;
    std::cout << std::dec << s9  << std::endl;

    return 0;
}

int mainXXA()
{
    byte_t v0 = 1;
    byte_t v1 = 0;
    byte_t v2 = 1;
    byte_t v3 = 0;
    byte_t v4 = 0;
    byte_t v5 = 0;
    byte_t v6 = 1;
    byte_t v7 = 1;

    byte_t  out = 0;
    out |= (v0 << 7) | (v1 << 6) | (v2 << 5) | (v3 << 4) | (v4 << 3) | (v5 << 2) | (v6 << 1) | (v7 << 0);

    std::bitset<CHAR_BIT * sizeof(byte_t)> bs(out);
    std::cout << "hash : pos [" << std::dec  << std::hex << " 0x" << std::setw(16) << out << " : " <<  " :: " << bs << std::endl;

    //LBPMatcher::createLBPEnhanced("/home/gbugaj/share/devbox/data1/face004.tif", 0, 0);
    //LBPMatcher::createLBP("/home/gbugaj/share/devbox/data1/face004.tif");

    //LBPMatcher::createLBPEnhanced("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/27.png");
    //LBPMatcher::createLBPEnhanced("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/27.png", 1, 8);
    //LBPMatcher::createLBP("/home/gbugaj/share/devbox/data1/anna-300x300.jpg");
    //LBPMatcher::createLBPEnhanced("/home/gbugaj/share/devbox/data1/anna-300x300.jpg", 0, 0);
    //auto lbp = LBPMatcher::createLBP("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/clion.png");
  //  return 0;
}

int mainDIFF()
{
//    auto h1 = ImageHash::hash("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/44.png", ImageHash::DIFFERENCE);
//    auto h2 = ImageHash::hash("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/45.png", ImageHash::DIFFERENCE);

    //auto h1 = ImageHash::hash("/home/gbugaj/dev/lbp-matcher/test-deck/template-processed/0.png", ImageHash::DIFFERENCE);
    //auto h2 = ImageHash::hash("/home/gbugaj/dev/lbp-matcher/test-deck/template-processed/4.png", ImageHash::DIFFERENCE);

     auto h1 = ImageHash::hash("/home/gbugaj/share/devbox/hashing/zeros/29.png", ImageHash::DIFFERENCE);
     auto h2 = ImageHash::hash("/home/gbugaj/share/devbox/hashing/zeros/56.png", ImageHash::DIFFERENCE);


    HashDistance hs ;
    auto val = hs.distance(h1, h2);
    auto norm = hs.normalized(h1, h2);

    std::cout<< "Distance   = " << val << "\n";
    std::cout<< "Normalized = " << norm << "\n";

    return 0;
}

int mainaxxa()
{
    fs::path path = fs::path("/home/gbugaj/share/devbox/hashing/001/");

    for(auto& file : fs::directory_iterator(path))
    {
        std::string p = file.path();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        auto h1 = ImageHash::hash(p, ImageHash::DIFFERENCE);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        std::cout<< h1  << ", "  << duration << ", " <<p <<std::endl;
    }

    return 0;
}


hash_t bucket(hash_t key, const std::set <hash_t> & keys)
{
    HashDistance hs ;
    hash_t maxh = key;
    double maxnorm = .75;

    for(auto it = keys.begin(); it != keys.end(); ++it)
    {
        hash_t key2 = (*it);
        auto val = hs.distance(key, key2);
        auto norm = hs.normalized(key, key2);
        if(norm > maxnorm && key!= key2)
        {
           // std::cout<< "    - "<< std::hex << "0x" << key2 <<  " : " << std::dec << val <<  " :: " << norm << std::endl;
            maxnorm = norm;
            maxh = key2;
        }
    }

  return maxh;
}

int mainsdfsdf()
{
    //fs::path path = fs::path("/home/gbugaj/share/devbox/hashing/001/");
    fs::path path = fs::path("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01");

    std::multimap<hash_t, std::string> map;
    std::set <hash_t> set;

    for(auto& file : fs::directory_iterator(path))
    {
        std::string p = file.path();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        auto h1 = ImageHash::hash(p, ImageHash::DIFFERENCE);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        std::cout<< std::hex <<"0x" << h1  << ", "<< std::dec << duration << ", " <<p <<std::endl;

        map.insert(std::pair(h1, p));
        set.insert(h1);
    }

    //auto key = ImageHash::hash("/home/gbugaj/share/devbox/hashing/zeros/23.png", ImageHash::DIFFERENCE);
    auto key = ImageHash::hash("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/19.png", ImageHash::DIFFERENCE);


    HashDistance hs ;
    double maxnorm = .60;
    hash_t maxh = 0;
    for(auto it = set.begin(); it != set.end(); ++it)
    {
        hash_t key2 = (*it);
        auto val = hs.distance(key, key2);
        auto norm = hs.normalized(key, key2);
        if(norm > maxnorm)
        {
            std::cout<< "    - "<< std::hex << "0x" << key2 <<  " : " << std::dec << val <<  " :: " << norm << std::endl;
            maxnorm = norm;
            maxh = key2;
        }
    }

    auto search = map.find(maxh);
    if(search != map.end())
    {
        std::cout << "Found " << search->first << " " << search->second << '\n';
    }

    if(true)
        return 1;


    // Cluster into buckets
    std::multimap<hash_t, std::string> clusters;
    for(auto it = set.begin(); it != set.end();)
    {
        hash_t key = (*it);
        hash_t bck = bucket(key, set);
        std::cout<< std::hex << "0x" << key  <<" :: " << bck<< std::endl;

        set.erase(it);
        ++it;
    }


    for (auto itr = map.begin(); itr != map.end(); )
    {
        auto val = (*itr);
        auto key = val.first;
        std::cout<<  key << std::endl;

        /* Now, go skip to the first entry with a new key. */
        auto curr = itr;
        while (itr != map.end() && itr->first == curr->first)
        {
            auto kval = (*itr);
            auto file = kval.second;
            std::cout<< "    " << file << std::endl;
            ++itr;
        }
    }


    std::cout<<  "-------------------- " << std::endl;

    for (auto itr = map.begin(); itr != map.end(); )
    {
        auto val = (*itr);
        auto key = val.first;
        std::cout<<  key << std::endl;

        /* Now, go skip to the first entry with a new key. */
        auto curr = itr;
        while (itr != map.end() && itr->first == curr->first)
        {
            auto kval = (*itr);
            auto file = kval.second;
            std::cout<< "    " << file << std::endl;
            ++itr;
        }
    }

    return 0;
}


int mainxas()
{
    auto h1 = ImageHash::hash("/home/gbugaj/share/devbox/hashing/002/16.png", ImageHash::DIFFERENCE);
    auto h2 = ImageHash::hash("/home/gbugaj/share/devbox/hashing/002/17.png", ImageHash::DIFFERENCE);

    std::cout<< "Hashing result = " << std::hex <<"0x"<< h1 << "\n";
    std::cout<< "Hashing result = " << std::hex <<"0x"<< h2 << "\n";

    HashDistance hs ;
    auto val = hs.distance(h1, h2);
    auto norm = hs.normalized(h1, h2);

    std::cout<< "Distance   = " << std::dec << val <<  " :: " << norm << "\n";

    return 0;
}


int main()
{
 //   ImageSimilarity::similarity("/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/42.png", "/home/gbugaj/dev/lbp-matcher/test-deck/deck-01/42.png", ImageHash::AVERAGE);

    LBPModel model(4);
    LBPModel sample(4);

    model.bins[0] = 1;
    model.bins[1] = 2;
    model.bins[2] = 3;
    model.bins[3] = 4;

    sample.bins[0] = 1;
    sample.bins[1] = 2;
    sample.bins[2] = 4;
    sample.bins[3] = 4;

    HistogramComparison histogram;

    auto r1 = histogram.compare(model, sample,  HistogramComparison::CompareType::INTERSECTION);
    auto r2 = histogram.compare(model, sample,  HistogramComparison::CompareType::CHI_SQUARED);
    auto r3 = histogram.compare(model, sample,  HistogramComparison::CompareType::LOG_LIKELIHOOD);
    auto r4 = histogram.compare(model, sample,  HistogramComparison::CompareType::KULLBACK_LEIBLER_DIVERGENCE);


    std::cout<< "---------------------------------\n";
    std::cout<< "Intersection      = " << r1 << "\n";
    std::cout<< "Chi Squared       = " << r2 << "\n";
    std::cout<< "LogLikehood       = " << r3 << "\n";
    std::cout<< "Kullback-Leibler  = " << r4 << "\n";

    return 0;
}