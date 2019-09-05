#ifndef LBP_MATCHER_EXTRACTOR_H
#define LBP_MATCHER_EXTRACTOR_H

#include <string>
#include "fileutil.h"

/**
 * https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_template_matching/py_template_matching.html
 * http://paulbourke.net/miscellaneous/correlate/
 */
class Extractor
{
public :

    /**
    * Compares a template against given document
    *
    * @param document the document to extract information from
    * @param snippet the template to search for
    * @return grayscale image, where each pixel denotes how much does the neighbourhood of that pixel match with template
    */
    PIX* extract(PIX* document, PIX* snippet);

    /**
     * Compares a template against given document
     *
     * @param document the document to extract information from
     * @param snippet the template to search for
     * @return grayscale image, where each pixel denotes how much does the neighbourhood of that pixel match with template
     */
    PIX* extract(const std::string& document, const std::string& snippet);
};

#endif //LBP_MATCHER_EXTRACTOR_H
