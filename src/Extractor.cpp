#include <leptonica/allheaders.h>
#include <iostream>
#include "Extractor.h"
#include "Segmenter.h"
#include "leptonutil.h"
#include "HashDistance.h"
#include "ImageHash.h"
#include "LBPMatcher.h"
#include "HistogramComparison.h"


PIX* Extractor::extract(const std::string &document, const std::string &templ)
{
    validateFileExists(document);
    validateFileExists(templ);

    PIX* src = pixRead(document.c_str());
    PIX* snip = pixRead(templ.c_str());
    PIX* data = extract(src, snip);

    pixDestroy(&src);
    pixDestroy(&snip);

    return data;
}

PIX*  Extractor::extract(PIX* document, PIX* templ)
{
    PIX* documentNorm = normalize(document);
    PIX* templNorm    =  normalize(templ);

    pixWritePng("/tmp/lbp-matcher/documentNorm.png", documentNorm, 0);
    pixWritePng("/tmp/lbp-matcher/templNorm.png", templNorm, 0);

    // Scale our template
    int dw = documentNorm->w;
    int dh = documentNorm->h;

    int bw = templ->w  ;
    int bh = templ->h  ;

    std::cout << "Extract  : "<< dw << ", " << dh << "  ::  " << bw <<" , " <<bh <<"\n";

    Segmenter seg;
    auto segments = seg.segment(dw, dh, bw, bh);
    auto m0 = LBPMatcher::createLBP(templ);

    HistogramComparison comp;
    auto type = HistogramComparison::CompareType::COSINE_SIMILARITY;
    std::cout << "\n-----------------------\n";

    int counter = 0 ;
    for(auto& segment: segments)
    {
        BOX* box = boxCreate(std::max(0, segment.x), std::max(0, segment.y), segment.w, segment.h);
        //std::cout << segment << "\n";

        if(box == NULL)
            continue;

        PIX* snip = pixClipRectangle(documentNorm, box, NULL);
        byte_t grayValue = 0;
        //std::cout << segment << "\n";

        if(snip !=  NULL)
        {
            char f1[255];
       /*     sprintf(f1, "/tmp/lbp-matcher/patch-%d.png", counter);
            pixWritePng(f1, snip, 0);
            ++counter;*/

           // if(counter > 600)
             //   break;
            auto m1 = LBPMatcher::createLBP(snip);
           // auto type = HistogramComparison::CompareType::CHI_SQUARED;
            //auto s0 = comp.compare(m0, m1, type);
            //grayValue = s0 * 255;

            //pixAtSet(bumpmap, segment.col, segment.row, grayValue);
          //  std::cout<<"\nROW : " << segment.row << "," <<  segment.col << " , "<< s0 <<" ," << (int)grayValue<< "\n";

            /*
            char f[255];
            sprintf(f, "/home/gbugaj/share/devbox/tmp/extractor/segment-%d-%d.png", segment.row, segment.col);
            pixWritePng(f, snip, 0);
            */
        }

        boxDestroy(&box);
        pixDestroy(&snip);
    }

        //pixWritePng("/home/gbugaj/share/devbox/tmp/bumpmap.png", bumpmap, 0);


    return NULL;
}
