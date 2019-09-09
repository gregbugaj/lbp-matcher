#include "Ilbp.hpp"

ILBP::ILBP() {
    std::cout<<"Constructor ILBP" << std::endl;
}

void ILBP::generate(int **matrix, PIX *pix) const {
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
            double m =  (p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8) / 8;
            out |=   ((p1 > m) << 7)
                     | ((p2 > m) << 6)
                     | ((p3 > m) << 5)
                     | ((p4 > m) << 4)
                     | ((p5 > m) << 3)
                     | ((p6 > m) << 2)
                     | ((p7 > m) << 1)
                     | ((p8 > m) << 0);

            matrix[y - pad][x - pad] = out;
        }
    }

    pixDestroy(&padded);
    delete[] image;
}

ILBP::~ILBP() {
    std::cout<<"Destructor ILBP" << std::endl;
}