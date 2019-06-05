#include <iostream>
#include <algorithm>
#include <climits>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <set>
#include <iterator>
#include <complex>
#include <vector>

#include "LBPMatcher.h"
#include "leptonutil.h"
#include "low.h"
#include <chrono>

using namespace std::chrono;

typedef std::vector<std::pair<int_t, int_t>> neighbor_list;

void clamp(std::pair<int_t, int_t>& p, const int& w,const int& h)
{
    p.first = std::min(std::max(p.first, 0), w - 1);
    p.second = std::min(std::max(p.second, 0), h - 1);
}

// typical configurations
// P, R = (8, 1), (16, 2) and (8, 2)
neighbor_list neighbors(int_t x, int_t y, int_t radius, int_t points)
{
    std::vector<std::pair<int_t, int_t>> out;
    for (int p = 0; p < points; ++p)
    {
        // If we don't round we would need to use use bilinear interpolation without rounding
        double t = (2 * M_PI * p) / points;
        int xp = round(x + radius * cos(t));
        int yp = round(y - radius * sin(t));

        out.push_back(std::make_pair(xp, yp));
    }
    return out;
}

// Statics

LBPModel LBPMatcher::createLBPEnhanced(PIX* pix, int_t points, int_t radius)
{
    LBPModel model(256);
    std::cout<<"pix->d = "<< pix->d << std::endl;
    // convert color(32) to grayscale
    PIX* pdata = pixConvertTo8(pix, 0);
 //   pdata = pixThreshold8(pdata, 8, 32, 0);

    pdata = reduce(pix, 400, 0);
    pdata = pixConvertTo8(pdata, 0);
//    pdata = reduce(pdata, 500, 300);

//    PIX* pdata = normalize(pix);
    pixWritePng("/tmp/lbp-matcher/lbp-pdata.png", pdata, 1);
    std::cout<<"pix->d = "<< pdata->d << std::endl;

    int_t w = pdata->w;
    int_t h = pdata->h;
    int_t d = pdata->d;

    PIX* pixout = pixCreate(w, h, 8);

    // We assume 300 dpi
    pixSetResolution(pixout, 300, 300);

    for (int_t y = 0; y < h; ++y)
    {
        for (int_t x = 0; x < w; ++x)
        {
            l_uint32 p0 = pixAtGet(pdata, x,     y    ); // Center

            l_uint32 p1 = pixAtGet(pdata, x - 1, y - 1);
            l_uint32 p2 = pixAtGet(pdata, x,     y - 1);
            l_uint32 p3 = pixAtGet(pdata, x + 1, y - 1);

            l_uint32 p4 = pixAtGet(pdata, x + 1, y    );

            l_uint32 p5 = pixAtGet(pdata, x + 1, y + 1);
            l_uint32 p6 = pixAtGet(pdata, x    , y + 1);
            l_uint32 p7 = pixAtGet(pdata, x - 1, y + 1);

            l_uint32 p8 = pixAtGet(pdata, x - 1, y    );
            double mean =  (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8) / 8;

            byte_t  out = 0;

            // normal
                 out |=   ((p0 > p1) << 7)
                     | ((p0 > p2) << 6)
                     | ((p0 > p3) << 5)
                     | ((p0 > p4) << 4)
                     | ((p0 > p5) << 3)
                     | ((p0 > p6) << 2)
                     | ((p0 > p7) << 1)
                     | ((p0 > p8) << 0);

            // enhanced
           /* out |=     ((mean > p1) << 7)
                     | ((mean > p2) << 6)
                     | ((mean > p3) << 5)
                     | ((mean > p4) << 4)
                     | ((mean > p5) << 3)
                     | ((mean > p6) << 2)
                     | ((mean > p7) << 1)
                     | ((mean > p8) << 0);
*/
            pixAtSet(pixout, x, y, out);
        }
    }

    pixWritePng("/tmp/lbp-matcher/lbp-enhanced.png", pixout, 1);
    return model;
}

l_uint32 pixAtGetSan(PIX* pix, int_t x, int_t y, int_t w, int_t h)
{
    if(x < 0 || x > w || y < 0 || y > h)
        return 0;

    l_int32 wpl    = pixGetWpl(pix);
    l_uint32* data = pixGetData(pix);
    l_uint32* line = data + y * wpl;
    l_uint32 value = GET_DATA_BYTE(line, x);

    return value;
}


enum MicroTexton
{
    SPOT      = 0x00, // 0000 0000
    SPOT_FLAT = 0xFF, // 1111 1111
    LINE_END  = 0x30, // 0011 0000
    EDGE      = 0xE1, // 1110 0001
    CORNER    = 0x70  // 0111 0000
};

LBPModel LBPMatcher::createLBP(PIX *pix)
{
    int padL = 1;
    int padR = 1;
    int padT = 1;
    int padB = 1;

    if(true)
    {
        createLBPEnhanced(pix, 0, 0);

        LBPModel ret(1);
        std::cout << ret << std::endl;
        return ret;
    }

    PIX* pdata = normalize(pix, 0, 0, 0, 0);

    int_t w = pdata->w;
    int_t h = pdata->h;

    // Horizontal * Vertical blocks
    int_t hbin = 1;
    int_t vbin = 1;

    int binsize = 256;
    int bins = hbin * vbin;
    int nbin = bins * binsize;

    int hs = ceil((double) h / (double) hbin);
    int ws = ceil((double) w / (double) vbin);

    /*
    std::cout<<"\n hbin = "<< hbin;
    std::cout<<"\n wbin = "<< vbin;
    std::cout<<"\n bins = "<< bins;
    std::cout<<"\n hs = "<< hs;
    std::cout<<"\n ws = "<< ws;
    std::cout<<"\n nbin = "<< nbin;
     */

    LBPModel featureHistogram(nbin);

    PIX* pixUniform = pixCreate(w, h, 8);
    PIX* pixNonUniform = pixCreate(w, h, 8);

    pixSetResolution(pixUniform, 300, 300);
    pixSetResolution(pixNonUniform, 300, 300);

    // Uniform descriptors
    // 0000 0000  (0 Transitions : Uniform)    0x0
    // 1110 0011  (2 Transitions : Uniform)    0xE3
    // 0101 0000  (4 Transitions : NonUniform) 0x50
    // 0000 1010  (4 Transitions : NonUniform) 0xA
    // 0000 1001  (3 Transitions : NonUniform) 0x9

    int index = 0;
    std::cout <<"\n";
    std::set <int> unique;

    int a = 2;

    for (int row = 0; row < vbin; ++row)
    {
        for (int col = 0; col < hbin; ++col)
        {
            index = row * vbin + col;
            int offset = index * binsize;
            int ystart = row * hs;
            int xstart = col * ws;

//          std::cout<<" indexs = "<< index << " pos : " << ystart << ":"<<ystart << ", offset : " << offset << std::endl;
            for (int_t y =  std::max(1, ystart), yend = std::min(ystart + hs, h - 1); y < yend; ++y)
            {
                for (int_t x = std::max(1, xstart), xend = std::min(xstart + ws, w - 1); x < xend; ++x)
                {
                    l_int32 p0 = pixAtGetSan(pdata, x,     y    , w , h); // Center
                    l_int32 p1 = pixAtGetSan(pdata, x - 1, y - 1, w , h);
                    l_int32 p2 = pixAtGetSan(pdata, x,     y - 1, w , h);
                    l_int32 p3 = pixAtGetSan(pdata, x + 1, y - 1, w , h);
                    l_int32 p4 = pixAtGetSan(pdata, x + 1, y    , w , h);
                    l_int32 p5 = pixAtGetSan(pdata, x + 1, y + 1, w , h);
                    l_int32 p6 = pixAtGetSan(pdata, x    , y + 1, w , h);
                    l_int32 p7 = pixAtGetSan(pdata, x - 1, y + 1, w , h);
                    l_int32 p8 = pixAtGetSan(pdata, x - 1, y    , w , h);

                    l_uint8 m =  (p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8) / 9;

                    l_uint8 out = 0;
                    out += sign(p1, p0 + a) * std::pow(2, 0);
                    out += sign(p2, p0 + a) * std::pow(2, 1);
                    out += sign(p3, p0 + a) * std::pow(2, 2);
                    out += sign(p4, p0 + a) * std::pow(2, 3);
                    out += sign(p5, p0 + a) * std::pow(2, 4);
                    out += sign(p6, p0 + a) * std::pow(2, 5);
                    out += sign(p7, p0 + a) * std::pow(2, 6);
                    out += sign(p8, p0 + a) * std::pow(2, 7);

        /*
                    l_uint8  out = 0;
                    out |=   ((p0 > p1) << 7)
                           | ((p0 > p2) << 6)
                           | ((p0 > p3) << 5)
                           | ((p0 > p4) << 4)
                           | ((p0 > p5) << 3)
                           | ((p0 > p6) << 2)
                           | ((p0 > p7) << 1)
                           | ((p0 > p8) << 0);
*/
                    // Check if the bit string is a uniform pattern
                    auto uniform = false;
                    auto tran = transition_lbp(out);

                    if(tran <= 2)
                        uniform = true;

                    if(false)
                    {
                        std::bitset<CHAR_BIT * sizeof(byte_t)> bs(out);
                        std::bitset<CHAR_BIT * sizeof(byte_t)> bsv(m);

                        std::cout << "pos [" << std::dec << std::setw(4) << x <<"," << std::setw(4) << y << "] " << (int)p0 << " :: "<<(int) m << " 0x"

                                  << std::setw(8) << std::hex  << (int) out << " : "
                                  << std::setw(4) << std::dec << (int)out << " :: " << bs <<  "  variance " <<  bsv <<" trans : " <<tran<< std::endl;
                    }

                    ++featureHistogram.bins[offset + static_cast<int>(out)];

                    if(uniform)
                    {
                        unique.insert((unsigned int)out);
                        //std::cout<< (unsigned int) out <<"|";
                        pixAtSet(pixUniform, x, y, out);
                    }
                    else
                    {
                        pixAtSet(pixNonUniform, x, y, out);
                    }
                }
            }

            if(false)
            {
                BOX*  box    = boxCreate(xstart,ystart,ws, hs);
                PIX*  snip   = pixClipRectangle(pix, box, NULL);

                char f[255];
                sprintf(f, "/tmp/lbp-matcher/box-%d-%d.png", row, col);
                pixWritePng(f, snip, 0);
                boxDestroy(&box);
                pixDestroy(&snip);
            }
        }
    }

    pixWritePng("/home/gbugaj/share/devbox/data1/lbp-uniform.png", pixUniform, 1);
    pixWritePng("/home/gbugaj/share/devbox/data1/lbp-nonuniform.png", pixNonUniform, 1);

    pixDestroy(&pdata);
    pixDestroy(&pixUniform);
    pixDestroy(&pixNonUniform);

    //std::cout<<" \n Set : " << unique.size();
    //std::cout<<" Histogram : \n " << featureHistogram;

    /*
       std::cout<< " START \n ";
    for(int i =0, s = featureHistogram.size; i< s; ++i)
    {
        std::cout<< std::dec<< featureHistogram.bins[i] << "\n";
    }
     */
    return featureHistogram;
}

int LBPMatcher::sign(l_int32 point, l_int32  center)
{
    return (point - center) >= 0 ? 1 : 0;
}

LBPModel LBPMatcher::createLBP(const std::string &filename)
{
    std::cout << "reateLBP reading file : " << filename << std::endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    validateFileExists(filename);

    PIX* pix = pixRead(filename.c_str());
    auto model = createLBP(pix);
    pixDestroy(&pix);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    std::cout <<  "createLBP Time : "  << duration << std::endl;

    return model;
}

LBPModel LBPMatcher::createLBPEnhanced(const std::string &filename, int_t points, int_t radius)
{
    std::cout << "Reading file : " << filename;
    validateFileExists(filename);

    PIX* pix = pixRead(filename.c_str());
    auto model = createLBPEnhanced(pix, 0, 0);
    pixDestroy(&pix);

    return model;
}