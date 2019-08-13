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

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "heatmap.h"
#include "leptonutil.h"

using namespace std::chrono;
namespace  fs = std::experimental::filesystem;

void test_histogram_scores();
void test_lbp_001();
void test_lbp_003();
void test_lbp_000();
void test_lbp_patch();
void test_histogram_append();
void test_histogram_normalize();
void test_extractor_001();
void test_image_normalization();

void test_heatmap_001();

void test_histogram_outlier_removal();
void test_lbp_createlbpmatrix();

int main(int argc, char* argv[])
{

//    test_histogram_normalize();
//    test_histogram_append();
//    test_lbp_000();
//      test_lbp_001();
//    test_lbp_003();
//    test_histogram_scores();
    test_extractor_001();
//    test_heatmap_001();
    //test_image_normalization();

//    test_histogram_outlier_removal();

//    test_lbp_createlbpmatrix();
    return 0;
}

void test_segmenter_01()
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
}

fs::path getTestDeckDirectory(const std::string& folder)
{
    auto path = fs::current_path();
    path /= "../test-deck";
    path /= folder;
    return path;
}

/***
 * upscale the document
 */
void test_image_normalization()
{
    auto deck = getTestDeckDirectory("private");
    auto document = deck / "clip3.tif";
    auto snip = deck / "0.png";

    auto pix = pixRead(document.c_str());
    auto upscale = pixUpscaleToGray(pix);

    char f[255];
    sprintf(f, "/tmp/lbp-matcher/upscale.png");
    pixWritePng(f, upscale, 0);
}

void test_heatmap_001()
{
    auto deck = getTestDeckDirectory("heat");
    auto colorscale = deck / "colorscale_jet.jpg";
    /*auto pix = pixReadJpeg (colorscale.c_str(), 1, 1, nullptr, 0);
    heatmapGenerateLookupTable(pix);*/

//    Pix* p = pixRead("/tmp/lbp-matcher/bumpmap.png");
    Pix* p = pixRead("/tmp/lbp-matcher/height-map-surface-representation.png");
    heatmap(p);
}

void test_extractor_001()
{
    auto deck = getTestDeckDirectory("private");
    auto document = deck / "clip2.tif";
    auto snip = deck / "patch-1185.png";

    Extractor extractor;
    extractor.extract(document, snip);
}

void test_lbp_createlbpmatrix()
{
    auto deck = getTestDeckDirectory("deck-01");
    auto f1 = deck / "27.png";

    std::cout <<"Test deck dir : " << deck << std::endl;
    std::cout <<"Test f1 : " << f1 << std::endl;

    PIX* pix1 = pixUpscaleToGray(f1.c_str());

    auto m0 = LBPMatcher::createLBP(pix1);
    std::cout << "Histograms " << std::endl;
    std::cout << m0 << std::endl;
}

void test_lbp_000()
{
//    auto deck = getTestDeckDirectory("template-claim");
//    auto f1 = deck / "0.png";
//
//    auto f2 = deck / "27_scale_125_.png";
/*    auto deck = getTestDeckDirectory("set-color");
    auto f1 = deck / "194045.jpg";*/

    auto deck = getTestDeckDirectory("deck-01");
    auto f1 = deck / "27.png";

    std::cout <<"Test deck dir : " << deck << std::endl;
    std::cout <<"Test f1 : " << f1 << std::endl;

    PIX* pix1 = pixUpscaleToGray(f1.c_str());

    auto m0 = LBPMatcher::createLBP(pix1);
    std::cout << "Histograms " << std::endl;
    std::cout << m0 << std::endl;
}

void test_lbp_001()
{
    auto deck = getTestDeckDirectory("deck-patch-2");
    auto f1 = deck / "clip2-0.png";
    auto f2 = deck / "clip3-0.png";


//    auto f2 = deck / "best-1260-249.png";
    /*auto deck = getTestDeckDirectory("deck-01");
    auto f1 = deck / "1.png";
    auto f2 = deck / "2.png";
*/
    std::cout <<"Test deck dir : " << deck << std::endl;
    std::cout <<"Test f1 : " << f1 << std::endl;
    std::cout <<"Test f2 : " << f2 << std::endl;

    PIX* pix1 = normalize(pixRead(f1.c_str()));
    PIX* pix2 = normalize(pixRead(f2.c_str()));

    auto m0 = LBPMatcher::createLBP(pix1);
    auto m1 = LBPMatcher::createLBP(pix2);

    pixDestroy(&pix1);
    pixDestroy(&pix2);

    std::cout << "Histograms " << std::endl;
    std::cout << m0 << std::endl;

    HistogramComparison comp;
    auto type = HistogramComparison::CompareType::CHI_SQUARED;

    std::cout << "Histograms Raw " << std::endl;
    std::cout << m0 << std::endl;
    std::cout << m1<< std::endl;

    m0.normalizeOutliers();
    m1.normalizeOutliers();

    std::cout << "Histograms Outliers " << std::endl;
    std::cout << m0 << std::endl;
    std::cout << m1<< std::endl;

    m0.normalize();
    m1.normalize();

    auto s0 = comp.compare(m0, m0, type);
    auto s1 = comp.compare(m1, m1, type);
    auto s2 = comp.compare(m0, m1, type);
    auto s3 = comp.compare(m1, m0, type);

    std::cout << "Scores :" << std::endl;
    std::cout << "s0 : " << std::dec << s0  << std::endl;
    std::cout << "s1 : " << std::dec << s1  << std::endl;
    std::cout << "s2 : " << std::dec << s2  << std::endl;
    std::cout << "s3 : " << std::dec << s3  << std::endl;

    std::cout << "Histograms " << std::endl;
    std::cout << m0 << std::endl;
    std::cout << m1<< std::endl;

}

void test_lbp_002()
{
    auto deck = getTestDeckDirectory("template-processed");
    HistogramComparison comp;

    auto f1 = deck / "0.png";
    auto f2 = deck / "1.png";
    auto f3 = deck / "2.png";

    auto m0 = LBPMatcher::createLBP(f1);
    auto m1 = LBPMatcher::createLBP(f2);
    auto m2 = LBPMatcher::createLBP(f3);

    auto type = HistogramComparison::CompareType::INTERSECTION;

    auto s0 = comp.compare(m0, m0, type);
    auto s1 = comp.compare(m0, m1, type);
    auto s2 = comp.compare(m0, m2, type);

    std::cout << "\nScores :" << std::endl;
    std::cout << "s0 :" << std::dec << s0  << std::endl;
    std::cout << "s1 :" << std::dec << s1  << std::endl;
    std::cout << "s2 :" << std::dec << s2  << std::endl;
}

void test_lbp_003()
{
    auto deck = getTestDeckDirectory("template-claim");
    HistogramComparison comp;

    auto m = LBPMatcher::createLBP(deck / "9.png");

    auto m0 = LBPMatcher::createLBP(deck / "0.png");
    auto m1 = LBPMatcher::createLBP(deck / "1.png");
    auto m2 = LBPMatcher::createLBP(deck / "2.png");
    auto m3 = LBPMatcher::createLBP(deck / "3.png");
    auto m4 = LBPMatcher::createLBP(deck / "4.png");
    auto m5 = LBPMatcher::createLBP(deck / "5.png");
    auto m6 = LBPMatcher::createLBP(deck / "6.png");
    auto m7 = LBPMatcher::createLBP(deck / "7.png");
    auto m8 = LBPMatcher::createLBP(deck / "8.png");
    auto m9 = LBPMatcher::createLBP(deck / "9.png");

    auto type = HistogramComparison::CompareType::COSINE_SIMILARITY;

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
}


void test_hash_001()
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
}

void test_hash_002()
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

int test_hash_003()
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

void test_hash_004()
{
    auto h1 = ImageHash::hash("/home/gbugaj/share/devbox/hashing/002/16.png", ImageHash::DIFFERENCE);
    auto h2 = ImageHash::hash("/home/gbugaj/share/devbox/hashing/002/17.png", ImageHash::DIFFERENCE);

    std::cout<< "Hashing result = " << std::hex <<"0x"<< h1 << "\n";
    std::cout<< "Hashing result = " << std::hex <<"0x"<< h2 << "\n";

    HashDistance hs ;
    auto val = hs.distance(h1, h2);
    auto norm = hs.normalized(h1, h2);

    std::cout<< "Distance   = " << std::dec << val <<  " :: " << norm << "\n";
}

void test_histogram_outlier_removal()
{
    Histogram model(9);

    model[0] = 27;
    model[1] = 23;
    model[2] = 25;
    model[3] = 22;
    model[4] = 23;
    model[5] = 20;
    model[6] = 25;
    model[7] = 29;
    model[8] = 1110;

    std::cout<< "With outliers   = " << model <<  std::endl;

    model.normalizeOutliers();

    std::cout<< "Normalized outliers   = " << model <<  std::endl;
}

void test_histogram_scores()
{
    Histogram model(4);
    Histogram sample(4);

    model[0] = 20;
    model[1] = 20;
    model[2] = 30;
    model[3] = 40;

    sample[0] = 20;
    sample[1] = 20;
    sample[2] = 30;
    sample[3] = 40;

    model.normalize();
    sample.normalize();

    std::cout<< " model = " << model <<std::endl;
    std::cout<< " sample = " << sample <<std::endl;

    HistogramComparison histogram;

    auto r1 = histogram.compare(model, sample,  HistogramComparison::CompareType::INTERSECTION);
    auto r2 = histogram.compare(model, sample,  HistogramComparison::CompareType::CHI_SQUARED);
    auto r3 = histogram.compare(model, sample,  HistogramComparison::CompareType::LOG_LIKELIHOOD);
    auto r4 = histogram.compare(model, sample,  HistogramComparison::CompareType::KULLBACK_LEIBLER_DIVERGENCE);
    auto r6 = histogram.compare(model, sample,  HistogramComparison::CompareType::EUCLIDEAN_DISTANCE_NORMALIZED);
    auto r7 = histogram.compare(model, sample,  HistogramComparison::CompareType::ABSOLUTE_VALUE);
    auto r8 = histogram.compare(model, sample,  HistogramComparison::CompareType::COSINE_SIMILARITY);

    std::cout<< "---------------------------------\n";
    std::cout<< "Intersection          = " << r1 << "\n";
    std::cout<< "Chi Squared           = " << r2 << "\n";
    std::cout<< "LogLikehood           = " << r3 << "\n";
    std::cout<< "Kullback-Leibler      = " << r4 << "\n";
    std::cout<< "Euclidean normalized  = " << r6 << "\n";
    std::cout<< "Absolute Value        = " << r7 << "\n";
    std::cout<< "Cosine Similarity     = " << r8 << "\n";
}

void test_histogram_append()
{
    Histogram m1(3);
    Histogram m2(3);
    Histogram m3(3);

    m1[0] = 1;
    m1[1] = 2;
    m1[2] = 3;

    m2[0] = 4;
    m2[1] = 5;
    m2[2] = 6;

    m3[0] = 7;
    m3[1] = 8;
    m3[2] = 9;

    Histogram r(0);
    r.append(m1);
    r.append(m2);
    r.append(m3);

    std::cout<< "m1  = " << m1 << "\n";
    std::cout<< "m2  = " << m2 << "\n";
    std::cout<< "m3  = " << m3 << "\n";
    std::cout<< "r   = " << r << "\n";
}

void test_histogram_normalize()
{
    Histogram model(5);

    model[0] = 20.12123232;
    model[1] = 35.33342222888444;
    model[2] = 30.232322221111;
    model[3] = 40.00000000031313;
    model[4] = 10.00000000031313;

    std::cout<< "m1  = " << model << "\n";
    model.normalize();
    std::cout<< "m1  = " << model << "\n";
}