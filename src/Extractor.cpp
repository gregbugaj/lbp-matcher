#include <leptonica/allheaders.h>
#include <iostream>
#include <chrono>
#include "Extractor.h"
#include "Segmenter.h"
#include "leptonutil.h"
#include "LBPMatcher.h"
#include "HistogramComparison.h"
#include "heatmap.h"

using namespace std::chrono;

PIX* Extractor::extract(const std::string &document, const std::string &snippet)
{
    validateFileExists(document);
    validateFileExists(snippet);

    PIX* src = pixRead(document.c_str());
    PIX* snip = pixRead(snippet.c_str());
    PIX* match = extract(src, snip);

    pixDestroy(&src);
    pixDestroy(&snip);

    return match;
}


// solves: f(x) = n / 2^x
int_t calculate_reductions(int_t x)
{
    static const double log2   = 0.69314718055994530941723212145818;  // std::log(2)
    static const double logmin = 2.0794415416798359282516963643745;   // std::log(8)
    int_t val = std::max(0, std::min(3, (int_t) std::floor((std::log(x) - logmin) / log2)));
    return val;
}

PIX* reduce(PIX* pix, int_t reductions)
{
    if (pix == nullptr)
        return nullptr;

    PIX* local = pixClone(pix);
    int_t n    = reductions;

    while (n > 0)
    {
        local = pixScaleAreaMap2(local);
        if(!local)
            return nullptr;
        --n;
    }

    return local;
}


PIX* Extractor::extract(PIX* document, PIX* snippet)
{
    PIX* documentNorm = normalize(document);
    PIX* snippetNorm  = normalize(snippet);

    pixWritePng("/tmp/lbp-matcher/documentNorm.png", documentNorm, 0);
    pixWritePng("/tmp/lbp-matcher/snippetNorm.png", snippetNorm, 0);

    // reduce images if possible
    int_t reductions = 1 ;//std::min(2, calculate_reductions(std::min(documentNorm->w, documentNorm->h)));
    std::cout << " reductions  = " << reductions;

    int dw = documentNorm->w;
    int dh = documentNorm->h;
    int bw = snippet->w;
    int bh = snippet->h;

    std::cout << "Extract  : "<< dw << ", " << dh << "  ::  " << bw <<" , " <<bh <<"\n";
    PIX* bumpmap = pixCreate(dw, dh, 8);
    pixSetResolution(bumpmap, 300, 300);

    Segmenter seg;
    auto segments = seg.segment(dw, dh, bw, bh);

    // matrix that will be populated with LBP
    int** matrix = new int*[dh];
    for (int_t y = 0; y < dh; ++y)
        matrix[y] = new int[dw];

    LBPMatcher::createLBP(matrix, LbpType::ENHANCED, documentNorm);

    // dump matrix
    if(false)
    {
        std::cout << "DOCUMENT Matrix " << std::endl;
        std::cout << std::endl;
        for (int_t y = 0; y < dh; ++y)
        {
            std::cout << std::setw(3)  << y << " : ";
            for (int_t x = 0; x < dw; ++x)
            {
                auto val = matrix[y][x];
                std::cout  << std::setw(3) << val << " ";
            }
            std::cout << std::endl;
        }
    }

    char f2[255];
    sprintf(f2, "/tmp/lbp-matcher/extract-lbp.png");
    LBPMatcher::pixFromMatrix(matrix, dh, dw, f2);

    auto m0 = LBPMatcher::createLBP(snippetNorm);
    m0.normalizeOutliers();
    m0.normalize();
/*
    //    auto m2 = LBPMatcher::createLBP(snip);
    auto mhist = LBPMatcher::createLBPHistogram(matrix, dw, dh,1, 1, snippetNorm->w, snippetNorm->h);
    std::cout << "  SNIP :: " << m0 << std::endl;
    std::cout << "  HIST :: " << mhist << std::endl;
    std::cout << std::endl;
*/
    HistogramComparison comp;
    auto type = HistogramComparison::CompareType::CHI_SQUARED;

    double max = .75;
    Segmenter::Segment best;

    int counter = 0 ;
    int index = 0;

    auto size = segments.size();
    std::cout << "Total segment : " << size << std::endl;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    for(auto& segment: segments)
    {
        counter++;
        auto x = std::max(0, segment.x);
        auto y = std::max(0, segment.y);
        auto w = segment.w;
        auto h = segment.w;

        BOX* box =  boxCreate(std::max(0, segment.x), std::max(0, segment.y), segment.w, segment.h);
        if(!box)
        {
            std::cout << "Bad box ";
            continue;
        }

        auto m1= LBPMatcher::createLBPHistogram(matrix, dw, dh, box->x, box->y, box->w, box->h);
        m1.normalizeOutliers();
        m1.normalize();

        auto s0 = comp.compare(m0, m1, type);
        int grayValue = s0 * 255;
        pixAtSet(bumpmap, segment.x, segment.y, grayValue);

        if(s0 >= max)
        {
            max = s0;
            best = segment;

            BOX* box1 = boxCreate(std::max(0, best.x), std::max(0, best.y), best.w, best.h);
            PIX* snip1 = pixClipRectangle(documentNorm, box1, NULL);
            char f1[255];
            sprintf(f1, "/tmp/lbp-matcher/best-%d-%d-%d.png",  box->x,  box->y, grayValue);
            pixWritePng(f1, snip1, 1);
            counter++;

            boxDestroy(&box1);
            pixDestroy(&snip1);

            pixAtSet(bumpmap, segment.x, segment.y, grayValue);
            std::cout<<"FOUND : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";
        }

//        std::cout<<"ROW : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";
        boxDestroy(&box);
        ++index;
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();

    std::cout << "Extractor Time (ms) : "  << duration << std::endl;

    std::cout<<"\nMax : " << best.row << "," <<  best.col << " , "<< max <<" ," << std::endl;
    BOX* box = boxCreate(std::max(0, best.x), std::max(0, best.y), best.w, best.h);
    PIX* snip = pixClipRectangle(documentNorm, box, NULL);
    char f1[255];

    sprintf(f1, "/tmp/lbp-matcher/extracted-snip-%d-%d.png", best.x, best.y);
    pixWritePng(f1, snip, 1);
    pixWritePng("/tmp/lbp-matcher/bumpmap.png", bumpmap, 1);

    delete[] matrix;
    return NULL;
}