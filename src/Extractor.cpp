#include <leptonica/allheaders.h>
#include <iostream>
#include "Extractor.h"
#include "Segmenter.h"
#include "leptonutil.h"
#include "LBPMatcher.h"
#include "HistogramComparison.h"
#include "heatmap.h"

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

PIX* Extractor::extract(PIX* document, PIX* snippet)
{
    PIX* documentNorm = normalize(document);
    PIX* snippetNorm  = normalize(snippet);

    pixWritePng("/tmp/lbp-matcher/documentNorm.png", documentNorm, 0);
    pixWritePng("/tmp/lbp-matcher/snippetNorm.png", snippetNorm, 0);

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

    double max = .8;
    Segmenter::Segment best;

    int counter = 0 ;
    int index = 0;

    for(auto& segment: segments)
    {
        counter++;
        auto x = std::max(0, segment.x);
        auto y = std::max(0, segment.y);
        auto w = segment.w;
        auto h = segment.w;

        BOX* box =  boxCreate(std::max(0, segment.x), std::max(0, segment.y), segment.w, segment.h);
        if(!box)
            continue;

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
            sprintf(f1, "/tmp/lbp-matcher/best-%d-%d.png", counter, grayValue);
            pixWritePng(f1, snip1, 1);
            counter++;

            boxDestroy(&box1);
            pixDestroy(&snip1);

            pixAtSet(bumpmap, segment.x, segment.y, grayValue);
            std::cout<<"FOUND : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";
        }

//        std::cout<<"ROW : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";
        boxDestroy(&box);
//        pixDestroy(&snip);
        ++index;
    }

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



PIX* xxextract(PIX* document, PIX* snippet)
{
    PIX* documentNorm = normalize(document);
    PIX* snippetNorm  = normalize(snippet);

    pixWritePng("/tmp/lbp-matcher/documentNorm.png", documentNorm, 0);
    pixWritePng("/tmp/lbp-matcher/snippetNorm.png", snippetNorm, 0);

    int dw = documentNorm->w;
    int dh = documentNorm->h;
    int bw = snippet->w;
    int bh = snippet->h;

    std::cout << "Extract  : "<< dw << ", " << dh << "  ::  " << bw <<" , " <<bh <<"\n";
    PIX* bumpmap = pixCreate(dw, dh, 8);
    pixSetResolution(bumpmap, 300, 300);

    Segmenter seg;
    auto segments = seg.segment(dw, dh, bw, bh);

    // matrix that will be populated with LBP, data is in row-major order
    int** matrix = new int*[dh];
    for (int_t y = 0; y < dh; ++y)
        matrix[y] = new int[dw];

    LBPMatcher::createLBP(matrix, LbpType::ENHANCED, documentNorm);

    char f2[255];
    sprintf(f2, "/tmp/lbp-matcher/extract-lbp.png");
    LBPMatcher::pixFromMatrix(matrix, dh, dw, f2);

    auto m0 = LBPMatcher::createLBP(snippetNorm);

    m0.normalizeOutliers();
    m0.normalize();

    if(true)
        return nullptr;

    std::cout<<"m0 << " << m0 << std::endl;

    HistogramComparison comp;
    auto type = HistogramComparison::CompareType::CHI_SQUARED;

    double max = .8;
    Segmenter::Segment best;

    int counter = 0 ;
    int index = 0;

    for(auto& segment: segments)
    {
        counter++;
        BOX* box = boxCreate(std::max(0, segment.x), std::max(0, segment.y), segment.w, segment.h);
        if(!box)
            continue;

        PIX* snip = pixClipRectangle(documentNorm, box, NULL);
        if(!snip)
        {
            boxDestroy(&box);
            continue;
        }

        auto grayValue = 0;
        char f1[255];
        sprintf(f1, "/tmp/lbp-matcher/patch-%d.png", counter);
        //pixWritePng(f1, snip, 0);
        ++counter;

        auto m1 = LBPMatcher::createLBP(snip);
        m1.normalizeOutliers();
        m1.normalize();

        auto s0 = comp.compare(m0, m1, type);
        grayValue = s0 * 255;
        pixAtSet(bumpmap, segment.x, segment.y, grayValue);

        if(s0 >= max)
        {
            max = s0;
            best = segment;

            BOX* box1 = boxCreate(std::max(0, best.x), std::max(0, best.y), best.w, best.h);
            PIX* snip1 = pixClipRectangle(documentNorm, box1, NULL);
            char f1[255];
            sprintf(f1, "/tmp/lbp-matcher/best-%d-%d.png", counter, grayValue);
            pixWritePng(f1, snip1, 1);
            counter++;

            boxDestroy(&box1);
            pixDestroy(&snip1);

            pixAtSet(bumpmap, segment.x, segment.y, grayValue);
            std::cout<<"FOUND : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";
        }

        std::cout<<"ROW : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";
        boxDestroy(&box);
        pixDestroy(&snip);
        ++index;
    }

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
