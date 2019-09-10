#include "Elbp.hpp"

ELBP::ELBP() {
    std::cout << "Constructor ELBP" << std::endl;
}

void ELBP::generate(int **matrix, PIX *pix) const {
    std::cout << "Generate : ELBP" << std::endl;
    int radius = 2;
    int points = 8;

    // deal with padding
    l_int32 pad = radius;
    PIX *padded = pixAddBorderGeneral(pix, pad, pad, pad, pad, 255);
    int_t w = padded->w;
    int_t h = padded->h;

    l_int32 wpl = pixGetWpl(padded);
    l_uint32 *data = pixGetData(padded);
    int **image = new int *[h];

    for (int_t y = 0; y < h; ++y) {
        image[y] = new int[w];
        l_uint32 *line = data + y * wpl;
        for (int_t x = 0; x < w; ++x) {
            image[y][x] = GET_DATA_BYTE(line, x);
        }
    }

    for (int_t y = pad; y < h - pad; ++y) {
        for (int_t x = pad; x < w - pad; ++x) {

            int p0 = pixAtGetSan(image, h, w, x, y);
            int out = 0;

            for (int n = 0; n < points; ++n)
            {
                double t = (2 * M_PI * n) / points;
                double xx = x + radius * cos(t);
                double yy = y - radius * sin(t);

                int fx = static_cast<int>(floor(xx));
                int fy = static_cast<int>(floor(yy));
                int cx = static_cast<int>(ceil(xx));
                int cy = static_cast<int>(ceil(yy));

                // fractional part
                float ty = y - fy;
                float tx = x - fx;

                // set interpolation weights
                float w1 = (1 - tx) * (1 - ty);
                float w2 = tx * (1 - ty);
                float w3 = (1 - tx) * ty;
                float w4 = tx * ty;

                // data stores as LSB => MSB
                // 0000 0010 => 0100 0000 = 64
                // 0000 0001 => 1000 0000 =  128
                float p1 = w1 * pixAtGetSan(image, h, w, fx, fy);
                float p2 = w2 * pixAtGetSan(image, h, w, cx, fy);
                float p3 = w3 * pixAtGetSan(image, h, w, fx, cy);
                float p4 = w4 * pixAtGetSan(image, h, w, cx, cy);
                float total = p1 + p2 + p3 + p4;

                l_int32 v = (total > p0)  && (abs(total - p0) > std::numeric_limits<float>::epsilon());
//                l_int32 v = (abs(total - p0) > std::numeric_limits<float>::epsilon());
                out |= v << n;
            }

            out = std::min(out, 255);
            int ax = x - pad;
            int ay = y - pad;
            matrix[ay][ax] = out;
            //                std::cout << " ==> " << x << " : " << y << " | =  " <<  fx << " : " << fy << std::endl;
//            std::cout << " ==> " << ay << " : " << ax << "  =  " <<  (int)out << std::endl;
        }

   /*     if(true)
            break;*/
    }

    pixDestroy(&padded);
    delete[] image;
}

ELBP::~ELBP() {
    std::cout << "Destructor LBP" << std::endl;
}