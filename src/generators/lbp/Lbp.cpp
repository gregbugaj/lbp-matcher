#include "Lbp.hpp"

LBP::LBP() {
    std::cout<<"Constructor LBP" << std::endl;
}

void LBP::generate(int **matrix, PIX *pix) const {
    std::cout<<"Generate : LBP" << std::endl;
    // deal with padding
    l_int32  pad = 1;
    PIX* padded  = pixAddBorderGeneral(pix, pad, pad, pad, pad , 255);
    int_t w = padded->w;
    int_t h = padded->h;

    l_int32 wpl    = pixGetWpl(padded);
    l_uint32* data = pixGetData(padded);

    int** image = new int*[h];
    for (int_t y = 0; y < h; ++y)
        image[y] = new int[w];

    for (int_t y = 0; y < h; ++y)
    {
        l_uint32* line = data + y * wpl;
        for (int_t x = 0; x < w; ++x)
        {
            image[y][x] = GET_DATA_BYTE(line, x);
        }
    }

    for (int_t y = pad; y < h - pad; ++y)
    {
        for (int_t x = pad; x < w - pad; ++x)
        {
            l_int32 p0 = pixAtGetSan(image, h, w, x,     y    ); // Center
            l_int32 p1 = pixAtGetSan(image, h, w,  x - 1, y - 1);
            l_int32 p2 = pixAtGetSan(image, h, w,  x,     y - 1);
            l_int32 p3 = pixAtGetSan(image, h, w,  x + 1, y - 1);
            l_int32 p4 = pixAtGetSan(image, h, w,  x + 1, y    );
            l_int32 p5 = pixAtGetSan(image, h, w,  x + 1, y + 1);
            l_int32 p6 = pixAtGetSan(image, h, w,  x    , y + 1);
            l_int32 p7 = pixAtGetSan(image, h, w,  x - 1, y + 1);
            l_int32 p8 = pixAtGetSan(image, h, w,  x - 1, y    );

            byte_t  out = 0;

            out |=   ((p1 > p0) << 7)
                     | ((p2 > p0) << 6)
                     | ((p3 > p0) << 5)
                     | ((p4 > p0) << 4)
                     | ((p5 > p0) << 3)
                     | ((p6 > p0) << 2)
                     | ((p7 > p0) << 1)
                     | ((p8 > p0) << 0);

            matrix[y - pad][x - pad] = out;
        }
    }

    pixDestroy(&padded);
    delete[] image;
}

LBP::~LBP() {
    std::cout<<"Destructor LBP" << std::endl;
}