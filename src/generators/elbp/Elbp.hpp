#ifndef LBP_MATCHER_ELBP_HPP
#define LBP_MATCHER_ELBP_HPP

#include <LbpGenerator.hpp>

/**
 * Enhanced Local Binary Pattern
 *
 * 1) For every pixel(x, y) in an image, I, choose P neighboring pixels at a radius R.
 * 2) Calculate the intensity difference of the current pixel (x, y) with the P neighboring pixels.
 * 3. Threshold the intensity difference, such that all the negative differences are assigned 0 and all the positive differences are assigned 1, forming a bit vector.
 * 4. Convert the P-bit vector to its corresponding decimal value and replace the intensity value at  with this decimal value.
 */
class ELBP : public LBPGenerator  {
public:
    typedef std::vector<std::pair<int_t, int_t>> neighbor_list;

    ELBP();
    void generate(int** matrix, PIX* pix) const;
    ~ELBP();

protected:
    // typical configurations
    // P, R = (8, 1), (16, 2) and (8, 2)
    neighbor_list neighborscd(int_t x, int_t y, int_t radius, int_t points) const
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
};

#endif //LBP_MATCHER_ELBP_HPP
