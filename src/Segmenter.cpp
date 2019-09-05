#include <cmath>
#include <iostream>
#include "Segmenter.h"

#include <chrono>

using namespace std::chrono;

/*
std::vector<Segmenter::Segment> Segmenter::segment(int w, int h, int templw, int templh)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    auto segments = std::vector<Segment>();

    int hbin = w / 16;
    int vbin = h / 16;
    int bins = hbin * vbin;

    int hs = ceil((double) h / (double) hbin);
    int ws = ceil((double) w / (double) vbin);

    std::cout<<"Segments size : " << hbin <<", " << vbin << " :: " << bins << std::endl;

    for (int row = 0; row < vbin; ++row)
    {
        for (int col = 0; col < hbin; ++col)
        {
            int ystart = row * hs;
            int xstart = col * ws;

            Segment seg;

            seg.y = ystart;
            seg.x = xstart;
            seg.w = templw;
            seg.h = templh;
            seg.row = row;
            seg.col = col;

            segments.push_back(seg);
        }
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    std::cout<< "Segmenter time :  "  << duration << std::endl;

    return segments;
}
 */

std::vector<Segmenter::Segment> Segmenter::segment(int w, int h, int templw, int templh)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    auto segments = std::vector<Segment>();
    segments.reserve(w * h);

    // Create image patch that is the same size as the template
    // at each location x, y serving as a center
    int templHalfW = ceil((double) w / (double) templw);
    int templHalfH = ceil((double) h / (double) templh);

    int step = 1;

    for (int y = 0; y < h; y = y + step)
    {
        for (int x = 0; x < w; x = x + step)
        {
            Segment seg;
//            seg.x = x - templHalfW;
//            seg.y = y - templHalfH;
            seg.x = x;
            seg.y = y;

            seg.w = templw;
            seg.h = templh;

            seg.row = y;
            seg.col = x;

            segments.push_back(seg);
        }
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    std::cout<< "Segment time (ms) :  "  << duration << std::endl;

    return segments;
}
