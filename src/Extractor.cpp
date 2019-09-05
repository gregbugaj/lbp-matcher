#include <leptonica/allheaders.h>
#include <iostream>
#include <chrono>
#include "Extractor.h"
#include "Segmenter.h"
#include "leptonutil.h"
#include "LBPMatcher.h"
#include "HistogramComparison.h"
#include "heatmap.h"
#include "ImageHash.h"
#include "HashDistance.h"

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
        char f[255];
        sprintf(f, "/tmp/lbp-matcher/reduce-%d.png",n);
        pixWritePng(f, local, 0);

        if(!local)
            return nullptr;
        --n;
    }

    return local;
}

PIX* Extractor::extract(PIX* document, PIX* snippet)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    PIX* documentNorm = normalize(document);
    PIX* snippetNorm  = normalize(snippet);

    auto reductions = 0;// calculate_reductions(std::max(dw, dh));
    auto documentReduced = reduce(documentNorm, reductions);
    auto snippetReduced  = reduce(snippetNorm, reductions);
    auto documentReducedBitonal = pixConvertTo1(documentReduced, 127);
    auto snippetReducedBitonal  = pixConvertTo1(snippetReduced, 127);

    pixWritePng("/tmp/lbp-matcher/documentNorm.png", documentNorm, 0);
    pixWritePng("/tmp/lbp-matcher/reduced.png", documentReduced, 0);
    pixWritePng("/tmp/lbp-matcher/snippetNorm.png", snippetNorm, 0);
    pixWritePng("/tmp/lbp-matcher/snippetReduced.png", snippetReduced, 0);
    pixWritePng("/tmp/lbp-matcher/documentReducedBitonal.png", documentReducedBitonal, 0);
    pixWritePng("/tmp/lbp-matcher/snippetReducedBitonal.png", snippetReducedBitonal, 0);

    int dw = documentReduced->w;
    int dh = documentReduced->h;

    int bw = snippetReduced->w;
    int bh = snippetReduced->h;

    std::cout << "Extract  : "<< dw << ", " << dh << "  ::  " << bw <<" , " <<bh <<"\n";
    PIX* bumpmap = pixCreate(dw, dh, 8);
    pixSetResolution(bumpmap, 300, 300);

    Segmenter seg;
    auto segments = seg.segment(dw, dh, bw, bh);

    // matrix that will be populated with LBP
    int** matrix = new int*[dh];
    for (int_t y = 0; y < dh; ++y)
        matrix[y] = new int[dw];

    LBPMatcher::createLBP(matrix, LbpType::ENHANCED,  documentReduced);

    char f2[255];
    sprintf(f2, "/tmp/lbp-matcher/extract-lbp.png");
    LBPMatcher::pixFromMatrix(matrix, dh, dw, f2);

    auto m0 = LBPMatcher::createLBP(snippetReduced);
    m0.normalizeOutliers();
    m0.normalize();

    HistogramComparison comp;
    auto type = HistogramComparison::CompareType::CHI_SQUARED;
    std::cout<< "Template Snippet : " << m0 << std::endl;

    double max = .60;
    Segmenter::Segment best;

    int counter = 0 ;
    int counterXX = 0 ;
    int index = 0;

    auto size = segments.size();
    std::cout << "Total segment : " << size << std::endl;
    auto h2 = ImageHash::hash(snippetReduced, ImageHash::AVERAGE);

    l_int32 snippetConnections = 0;
    pixCountConnComp(snippetReducedBitonal, 8, &snippetConnections);

    l_int32 templateSnippetPixelCount = 0;
    pixCountPixels(snippetReducedBitonal, &templateSnippetPixelCount, nullptr);

    int half = templateSnippetPixelCount / 2;
    int lower = templateSnippetPixelCount - half;
    int upper = templateSnippetPixelCount + half;

    int halfConnection = snippetConnections / 2;
    int lowerConnection = snippetConnections - halfConnection;
    int upperConnection = snippetConnections + halfConnection;

    std::cout << "Snippet connected : " << snippetConnections << std::endl;
    std::cout << "On Pixels  : " << templateSnippetPixelCount << std::endl;
    std::cout << "half  : " << half << std::endl;
    std::cout << "lower  : " << lower << std::endl;
    std::cout << "upper  : " << upper << std::endl;
    std::cout << "lowerConnection  : " << lowerConnection << std::endl;
    std::cout << "upperConnection  : " << upperConnection << std::endl;

    for(auto& segment : segments)
    {
        counter++;
        auto x = std::max(0, segment.x);
        auto y = std::max(0, segment.y);
        auto w = segment.w;
        auto h = segment.w;

        pixAtSet(bumpmap, segment.x, segment.y, 0);

        BOX* box =  boxCreate(std::max(0, segment.x), std::max(0, segment.y), segment.w + 2, segment.h + 2);
        if(!box)
            continue;

        PIX*  snippetBitonal = pixClipRectangle(documentReducedBitonal, box, NULL);
        if(!snippetBitonal)
            continue;

        // apply heuristics to minimize search space
        l_int32 snippetPixelCount = 0;
        pixCountPixels(snippetBitonal, &snippetPixelCount, nullptr);

        if(snippetPixelCount == 0  || snippetPixelCount < lower || snippetPixelCount > upper)
        {
            pixDestroy(&snippetBitonal);
            continue;
        }

        l_int32 snippetConnection = 0;
        pixCountConnComp(snippetBitonal, 8, &snippetConnection);

        if(snippetConnection == 0 || snippetConnection < lowerConnection || snippetConnection > upperConnection)
        {
            pixDestroy(&snippetBitonal);
            continue;
        }

   /*     PIX* snip = pixClipRectangle(documentReduced, box, NULL);
        auto h1 = ImageHash::hash(snip, ImageHash::AVERAGE);
        HashDistance hs;
        auto norm = hs.normalized(h1, h2);
        if(norm < .60)
        {
          pixDestroy(&snip);
          continue;
        }

        pixDestroy(&snip);
        */

        ++index;

        auto m1 = LBPMatcher::createLBPHistogram(matrix, dw, dh, box->x, box->y, box->w, box->h);
        m1.normalizeOutliers();
        m1.normalize();

        auto s0 = comp.compare(m0, m1, type);
        int grayValue = s0 * 255;
        pixAtSet(bumpmap, segment.x, segment.y, grayValue);

        if(s0 >= max)
        {
            // verify via hashing
            PIX* snip = pixClipRectangle(documentReduced, box, NULL);
            auto h1 = ImageHash::hash(snip, ImageHash::AVERAGE);
            HashDistance hs;
            auto norm = hs.normalized(h1, h2);
            pixDestroy(&snip);

            max = s0;
            best = segment;
            int score = s0 * 100;

            BOX* box1 = boxCreate(std::max(0, best.x), std::max(0, best.y), best.w, best.h);
            PIX* snip1 = pixClipRectangle(documentReduced, box, NULL);

            char f1[255];
            sprintf(f1, "/tmp/lbp-matcher/best-%d-%d-%d.png",  box->x,  box->y, score);
            pixWritePng(f1, snip1, 1);
            counter++;

            boxDestroy(&box1);
            pixDestroy(&snip1);

            pixAtSet(bumpmap, segment.x, segment.y, grayValue);
            std::cout<<"FOUND : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue << " norm : " << norm<< "\n";
        }

        boxDestroy(&box);
        ++index;
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();

    std::cout << "counterXX  : "  << counterXX << std::endl;
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

PIX* extractAA(PIX* document, PIX* snippet)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    PIX* documentNorm = normalize(document);
    PIX* snippetNorm  = normalize(snippet);

    pixWritePng("/tmp/lbp-matcher/documentNorm.png", documentNorm, 0);
    pixWritePng("/tmp/lbp-matcher/snippetNorm.png", snippetNorm, 0);

    int dw = documentNorm->w;
    int dh = documentNorm->h;
    int bw = snippetNorm->w;
    int bh = snippetNorm->h;

    auto converted = pixConvertTo8(document, 0);
    auto reductions = 2;// calculate_reductions(std::max(dw, dh));
    auto reduced = reduce(converted, reductions);

    std::cout << "Extract  : "<< dw << ", " << dh << "  ::  " << bw <<" , " <<bh <<"\n";
    PIX* bumpmap = pixCreate(dw, dh, 8);
    pixSetResolution(bumpmap, 300, 300);

    Segmenter seg;
    auto segments = seg.segment(dw, dh, bw, bh);

    // matrix that will be populated with LBP
    int** matrix = new int*[dh];
    for (int_t y = 0; y < dh; ++y)
        matrix[y] = new int[dw];

    LBPMatcher::createLBP(matrix, LbpType::ENHANCED, reduced);

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

    HistogramComparison comp;
    auto type = HistogramComparison::CompareType::CHI_SQUARED;

    double max = .60;
    Segmenter::Segment best;

    int counter = 0 ;
    int counterXX = 0 ;
    int index = 0;

    auto size = segments.size();
    std::cout << "Total segment : " << size << std::endl;

    auto h2 = ImageHash::hash(f2, ImageHash::AVERAGE);

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

//        PIX*  snip = pixClipRectangle(documentNorm, box, NULL);
//        auto count = pixCountPixels(snip);
//        auto count = sum(snip);
//        std::cout<< "count = " << count << std::endl;

        if(true)
            continue;

        PIX*  snip = pixClipRectangle(documentNorm, box, NULL);
        auto h1 = ImageHash::hash(snip, ImageHash::AVERAGE);


        HashDistance hs ;
        auto val = hs.distance(h1, h2);
        auto norm = hs.normalized(h1, h2);
        pixAtSet(bumpmap, segment.x, segment.y, 50);
        if(norm > .75)
        {
            auto gv = norm * 255.0;
            pixAtSet(bumpmap, segment.x, segment.y, gv);
            ++counterXX;
        }
//        std::cout<< "Distance   = " << val << " :: " << norm << "\n";

        pixDestroy(&snip);
        ++index;
        if(true)
            continue;

        auto m1= LBPMatcher::createLBPHistogram(matrix, dw, dh, box->x, box->y, box->w, box->h);

        if(index % 10000)
            std::cout<<"ROW : " << segment.row << "," <<  segment.col << "\n";
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

        if(index % 10000)
            std::cout<<"ROW : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";

        boxDestroy(&box);
        ++index;
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();

    std::cout << "counterXX  : "  << counterXX << std::endl;
    std::cout << "Extractor Time (ms) : "  << duration << std::endl;
/*    std::cout<<"\nMax : " << best.row << "," <<  best.col << " , "<< max <<" ," << std::endl;
    BOX* box = boxCreate(std::max(0, best.x), std::max(0, best.y), best.w, best.h);
    PIX* snip = pixClipRectangle(documentNorm, box, NULL);
    char f1[255];

    sprintf(f1, "/tmp/lbp-matcher/extracted-snip-%d-%d.png", best.x, best.y);
    pixWritePng(f1, snip, 1);
  */

    pixWritePng("/tmp/lbp-matcher/bumpmap.png", bumpmap, 1);
    delete[] matrix;
    return NULL;
}