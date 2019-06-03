#include "fileutil.h"


void validateFileExists(const std::string &filename)
{
    std::ifstream f(filename);

    if (!f.good())
    {
        std::stringstream ss;
        ss << "File not found : "<< filename;

        throw std::runtime_error(ss.str());
    }
}