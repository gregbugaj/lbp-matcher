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

PIX*  Extractor::extract(PIX* document, PIX* snippet)
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

    auto m0 = LBPMatcher::createLBP(snippetNorm);
    m0.normalize();

    HistogramComparison comp;
    auto type = HistogramComparison::CompareType::CHI_SQUARED;

    // default to sensible min
    double max = .5;
    Segmenter::Segment best;

    int counter = 0 ;
    int index = 0;

    for(auto& segment: segments)
    {
        BOX* box = boxCreate(std::max(0, segment.x), std::max(0, segment.y), segment.w, segment.h);
        if(box == NULL)
            continue;

        PIX* snip = pixClipRectangle(documentNorm, box, NULL);

        if(snip !=  NULL)
        {
            int_t grayValue = 0;
             char f1[255];
             sprintf(f1, "/tmp/lbp-matcher/patch-%d.png", counter);
             //pixWritePng(f1, snip, 0);
             ++counter;

            auto m1 = LBPMatcher::createLBP(snip);
            m1.normalize();

            auto s0 = comp.compare(m0, m1, type);
            grayValue = s0 * 255;

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
            }
//
            pixAtSet(bumpmap, segment.x, segment.y, 50);

            if(grayValue > 180)
                pixAtSet(bumpmap, segment.x, segment.y, grayValue);

            std::cout<<"\nROW : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," <<grayValue<< "\n";
        }

        boxDestroy(&box);
        pixDestroy(&snip);

        ++index;
    }

    std::cout<<"\nMax : " << best.row << "," <<  best.col << " , "<< max <<" ," << std::endl;
    BOX* box = boxCreate(std::max(0, best.x), std::max(0, best.y), best.w, best.h);
    PIX* snip = pixClipRectangle(documentNorm, box, NULL);
    char f1[255];

    sprintf(f1, "/tmp/lbp-matcher/bestpatch-%d.png", max);

    pixWritePng(f1, snip, 1);

    pixWritePng("/tmp/lbp-matcher/bumpmap.png", bumpmap, 1);
    return NULL;
}
