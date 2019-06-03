#ifndef LBP_MATCHER_EXTRACTOR_H
#define LBP_MATCHER_EXTRACTOR_H

#include <string>
#include "fileutil.h"

class Extractor
{
public :

    /**
     * Compares a template against given document
     *
     * @param document
     * @param templ
     */
    void extract(PIX* document, PIX* templ);

    /**
     * Compares a template against given document
     *
     * @param document
     * @param templ
     */
    void extract(const std::string& document, const std::string& templ);

};


#endif //LBP_MATCHER_EXTRACTOR_H
