#include "Elbp.hpp"

ELBP::ELBP() {
    std::cout<<"Constructor ELBP" << std::endl;
}

void generateXX(int **matrix, PIX *pix)  {
    std::cout<<"Generate : ELBP" << std::endl;
    int radius = 2;
    int p = 16;

    /*auto np  = neighborscd(3, 3, radius, p);

    for(auto val : np)
    {
        std::cout<< " val " << val.first << " : " << val.second << std::endl;
    }*/
}

void ELBP::generate(int **matrix, PIX *pix) const {
    std::cout<<"Generate : ELBP" << std::endl;
    int radius = 1;
    int neighbors = 8;
    neighbors = std::max(std::min(neighbors, 31), 1);

    for (int n = 0; n < neighbors; n++) {
        // sample points
        float x = static_cast<float>(radius) * cos(2.0 * M_PI * n / static_cast<float>(neighbors));
        float y = static_cast<float>(radius) * -sin(2.0 * M_PI * n / static_cast<float>(neighbors));
        // relative indices
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));
        // fractional part
        float ty = y - fy;
        float tx = x - fx;
        // set interpolation weights
        float w1 = (1 - tx) * (1 - ty);
        float w2 = tx  * (1 - ty);
        float w3 = (1 - tx) *      ty;
        float w4 = tx  *      ty;

        std::cout << " n "<< n << " : " << fx<< " : " << fy << " w1 = " << w1 << std::endl;
    }

}

ELBP::~ELBP() {
    std::cout<<"Destructor LBP" << std::endl;
}