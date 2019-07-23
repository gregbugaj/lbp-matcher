#include <iostream>
#include <algorithm>
#include <climits>
#include <bitset>
#include <iomanip>

#include <set>
#include <iterator>
#include <complex>
#include <vector>

#include "LBPMatcher.h"
#include "leptonutil.h"
#include "low.h"
#include <chrono>
#include <strings.h>

using namespace std::chrono;

typedef std::vector<std::pair<int_t, int_t>> neighbor_list;

bool debug_box_pix = true;

void clamp(std::pair<int_t, int_t>& p, const int& w, const int& h)
{
    p.first = std::min(std::max(p.first, 0), w - 1);
    p.second = std::min(std::max(p.second, 0), h - 1);
}

int sign(l_int32 point, l_int32  center)
{
    return (point - center) >= 0 ? 1 : 0;
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

/**
 * Crate Local Binary Pattern image
 *
 * @param matrix the lbp matrix to populate
 * @param pix the pix to generate lbp for
 */
void generateLbpNormal(int** matrix, PIX* pix)
{
    int_t w = pix->w;
    int_t h = pix->h;

    for (int_t y = 0; y < h; ++y)
    {
        for (int_t x = 0; x < w; ++x)
        {
            l_int32 p0 = pixAtGetSan(pix, x,     y    , w , h); // Center
            l_int32 p1 = pixAtGetSan(pix, x - 1, y - 1, w , h);
            l_int32 p2 = pixAtGetSan(pix, x,     y - 1, w , h);
            l_int32 p3 = pixAtGetSan(pix, x + 1, y - 1, w , h);
            l_int32 p4 = pixAtGetSan(pix, x + 1, y    , w , h);
            l_int32 p5 = pixAtGetSan(pix, x + 1, y + 1, w , h);
            l_int32 p6 = pixAtGetSan(pix, x    , y + 1, w , h);
            l_int32 p7 = pixAtGetSan(pix, x - 1, y + 1, w , h);
            l_int32 p8 = pixAtGetSan(pix, x - 1, y    , w , h);

            byte_t  out = 0;

            out |=   ((p0 > p1) << 7)
                     | ((p0 > p2) << 6)
                     | ((p0 > p3) << 5)
                     | ((p0 > p4) << 4)
                     | ((p0 > p5) << 3)
                     | ((p0 > p6) << 2)
                     | ((p0 > p7) << 1)
                     | ((p0 > p8) << 0);

            matrix[y][x] = out;
        }
    }
}

/**
 * Crate Enhanced (improved) Local Binary Pattern image
 *
 * @param matrix the lbp matrix to populate
 * @param pix the pix to generate lbp for
 */
void generateLbpEnhanced(int** matrix, PIX* pix)
{
    int_t w = pix->w;
    int_t h = pix->h;

    for (int_t y = 0; y < h; ++y)
    {
        for (int_t x = 0; x < w; ++x)
        {
            l_int32 p0 = pixAtGetSan(pix, x,     y    , w , h); // Center
            l_int32 p1 = pixAtGetSan(pix, x - 1, y - 1, w , h);
            l_int32 p2 = pixAtGetSan(pix, x,     y - 1, w , h);
            l_int32 p3 = pixAtGetSan(pix, x + 1, y - 1, w , h);
            l_int32 p4 = pixAtGetSan(pix, x + 1, y    , w , h);
            l_int32 p5 = pixAtGetSan(pix, x + 1, y + 1, w , h);
            l_int32 p6 = pixAtGetSan(pix, x    , y + 1, w , h);
            l_int32 p7 = pixAtGetSan(pix, x - 1, y + 1, w , h);
            l_int32 p8 = pixAtGetSan(pix, x - 1, y    , w , h);

            byte_t  out = 0;
            double m =  (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8) / 8;
            out |=     ((m > p1) << 7)
                       | ((m > p2) << 6)
                       | ((m > p3) << 5)
                       | ((m > p4) << 4)
                       | ((m > p5) << 3)
                       | ((m > p6) << 2)
                       | ((m > p7) << 1)
                       | ((m > p8) << 0);

            matrix[y][x] = out;
        }
    }
}

/**
 * Crate Signed-Enhanced Local Binary Pattern image
 *
 * @param matrix the lbp matrix to populate
 * @param pix the pix to generate lbp for
 */
void generateLbpSigned(int** matrix, PIX* pix)
{
    int_t w = pix->w;
    int_t h = pix->h;
    int a = 2;

    for (int_t y = 0; y < h; ++y)
    {
        for (int_t x = 0; x < w; ++x)
        {
            l_int32 p0 = pixAtGetSan(pix, x,     y    , w , h); // Center
            l_int32 p1 = pixAtGetSan(pix, x - 1, y - 1, w , h);
            l_int32 p2 = pixAtGetSan(pix, x,     y - 1, w , h);
            l_int32 p3 = pixAtGetSan(pix, x + 1, y - 1, w , h);
            l_int32 p4 = pixAtGetSan(pix, x + 1, y    , w , h);
            l_int32 p5 = pixAtGetSan(pix, x + 1, y + 1, w , h);
            l_int32 p6 = pixAtGetSan(pix, x    , y + 1, w , h);
            l_int32 p7 = pixAtGetSan(pix, x - 1, y + 1, w , h);
            l_int32 p8 = pixAtGetSan(pix, x - 1, y    , w , h);

            byte_t  out = 0;

            out += sign(p1, p0 + a) * std::pow(2, 0);
            out += sign(p2, p0 + a) * std::pow(2, 1);
            out += sign(p3, p0 + a) * std::pow(2, 2);
            out += sign(p4, p0 + a) * std::pow(2, 3);
            out += sign(p5, p0 + a) * std::pow(2, 4);
            out += sign(p6, p0 + a) * std::pow(2, 5);
            out += sign(p7, p0 + a) * std::pow(2, 6);
            out += sign(p8, p0 + a) * std::pow(2, 7);

            matrix[y][x] = out;
        }
    }
}

void LBPMatcher::createLBP(int** matrix, LbpType type, PIX* pix)
{
    if(pix->d != 8)
        throw std::runtime_error("Pix BPP (bits per pixel) expected to be 8 but got " + std::to_string(pix->d));

    if(type == LbpType ::NORMAL)
        generateLbpNormal(matrix, pix);
    else if(type == LbpType ::ENHANCED)
        generateLbpEnhanced(matrix, pix);
    else if(type == LbpType ::SIGNED)
        generateLbpSigned(matrix, pix);
    else
        throw std::runtime_error("Unhandled lbp type");
}

bool isUniform(byte_t a)
{
    // Uniform descriptors
    // 0000 0000  (0 Transitions : Uniform)    0x0
    // 1110 0011  (2 Transitions : Uniform)    0xE3
    // 0101 0000  (4 Transitions : NonUniform) 0x50
    // 0000 1010  (4 Transitions : NonUniform) 0xA
    // 0000 1001  (3 Transitions : NonUniform) 0x9

    return transition_lbp(a) <= 2;
}

Histogram LBPMatcher::createLBP(PIX *pix)
{
    int padL = 1;
    int padR = 1;
    int padT = 1;
    int padB = 1;

    std::cout<<"pix->d = "<< pix->d << std::endl;
    PIX* normalized = normalize(pix);
    pixWritePng("/tmp/lbp-matcher/lbp-normalized.png", normalized, 1);
    std::cout<<"pix->d = "<< normalized->d << std::endl;

    int_t w = normalized->w;
    int_t h = normalized->h;

    // matrix that will be populated with LBP, data is in row-major order

    int** matrix = new int*[h];
    for (int_t y = 0; y < h; ++y)
        matrix[y] = new int[w];

    createLBP(matrix, LbpType::ENHANCED, normalized);

    // dump the lbp model
    if(debug_box_pix)
    {
        PIX* pixout = pixCreate(w, h, 8);
        pixSetResolution(pixout, 300, 300);
        for (int_t y = 0; y < h; ++y)
        {
            for (int_t x = 0; x < w; ++x)
            {
                pixAtSet(pixout, x, y, matrix[y][x]);
            }
        }
        pixWritePng("/tmp/lbp-matcher/lbp-enhanced.png", pixout, 1);
        pixDestroy(&pixout);
    }

    // Map the LBP codes to one of 58 uniform codes
    // Calculate uniform code table for all 255 grayscale codes
    // There are 58 codes and bin 59 is for everything else
    std::vector<int> uniforms;
    uniforms.resize(255);

    for(int i = 0, index = 0; i < 255; ++i)
    {
        if(isUniform(i))
            uniforms[i] = index++;
        else
            uniforms[i] = 59;
    }

    auto verticalPartitions = 2; // vertical  partitions
    auto horizontalPartitions = 4; // horizontal partitions

    // horizontal / vertical groups
    int gridHeight = ceil((double) h / (double) verticalPartitions);
    int gridWidth = ceil((double) w / (double) horizontalPartitions);

    std::cout<<" horizontalPartitions = "<< horizontalPartitions << std::endl;
    std::cout<<" verticalPartitions = "<< verticalPartitions << std::endl;
    std::cout<<" gridWidth = "<< gridWidth << std::endl;
    std::cout<<" gridHeight = "<< gridHeight << std::endl;

    Histogram result(0);

    for (int row = 0; row < verticalPartitions; ++row)
    {
        for (int col = 0; col < horizontalPartitions; ++col)
        {
            auto index  = row * horizontalPartitions + col;
            auto yStart = row * gridHeight;
            auto xStart = col * gridWidth;

            auto yEnd = std::min(yStart + gridHeight, h);
            auto xEnd = std::min(xStart + gridWidth, w);

            // dump region box
            if(debug_box_pix)
            {
                auto y = std::max(0, yStart);
                auto x = std::max(0, xStart);
                std::cout<<" ** index = "<< index <<  " row/col " << row  << " : " << col << " pos = " << y << ", "<< x  << " == "  <<  yEnd  << " : " << xEnd  <<std::endl;

                BOX*  box    = boxCreate(xStart, yStart, gridWidth, gridHeight);
                PIX*  snip   = pixClipRectangle(normalized, box, NULL);

                char f[255];
                sprintf(f, "/tmp/lbp-matcher/box-%d-%d.png", row, col);
                pixWritePng(f, snip, 0);

                boxDestroy(&box);
                pixDestroy(&snip);
            }

            Histogram boxModel(59);
            for (int_t y = std::max(0, yStart); y < yEnd; ++y)
            {
                for (int_t x = std::max(0, xStart); x < xEnd; ++x)
                {
                    auto out = matrix[y][x];
                    if(out == 0 || out == 255)
                        continue;

                    auto bin = uniforms[out];
                    boxModel[bin]++;
                }
            }

            result.append(boxModel);
        }
    }

    std::cout << " concat hist "  << result << std::endl;

    return result;
}

Histogram LBPMatcher::createLBP(const std::string &filename)
{
    std::cout << "createLBP reading file : " << filename << std::endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    validateFileExists(filename);

    auto pix = pixRead(filename.c_str());
    auto model = createLBP(pix);
    pixDestroy(&pix);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    std::cout <<  "createLBP Time : "  << duration << std::endl;

    return model;
}