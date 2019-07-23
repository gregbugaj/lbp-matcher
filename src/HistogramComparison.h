#ifndef LBP_MATCHER_HISTOGRAMCOMPARISON_H
#define LBP_MATCHER_HISTOGRAMCOMPARISON_H

#include <math.h>
#include <cmath>
#include "types.h"

class HistogramComparison
{

public :

    /**
     * Type of the metric to use for histogram comparison
     *
     * Similarity are measured in the range 0 to 1
     */
    enum CompareType
    {
        INTERSECTION,
        EUCLIDEAN_DISTANCE,
        EUCLIDEAN_DISTANCE_NORMALIZED,
        ABSOLUTE_VALUE,
        LOG_LIKELIHOOD,
        CHI_SQUARED,
        COSINE_SIMILARITY,
        KULLBACK_LEIBLER_DIVERGENCE
    };

    /**
     * Compare two histograms
     *
     * @param expected the model we comparing to
     * @param observed the sample we comparing agains
     * @param type Type of comparison
     * @return score in range 0..1, 0 = No similarity, 1 = Identical
     */
    double compare(const Histogram& expected, const Histogram& observed, CompareType type) const;

private :

    /**
     * Euclidean distance
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreEuclideanDistance(const Histogram& expected, const Histogram& observed) const;

    /**
     * Euclidian distance normalized
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreEuclideanDistanceNormalized(const Histogram& expected,const Histogram& observed) const;

    /**
     * Euclidean distance normalized
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreAbsoluteValueDistance(const Histogram& expected,const Histogram& observed) const;

    /**
     * Perform histogram intersection, value will be normalized in [0..1] range
     *
     * @param model
     * @param sample
     * @return
     */
    double scoreHistogramIntersection(const Histogram& expected,const Histogram& observed) const;

    /**
     * Perform log likehood scoring
     *
     * @param model
     * @param sample
     * @return
     */
    double scoreLogLikelihood(const Histogram &expected, const Histogram &observed) const;

    /**
     * Perform chi-squared goodness of fit test
     *
     * https://stats.stackexchange.com/questions/184101/comparing-two-histograms-using-chi-square-distance
     * <pre>
     * ∑i=1n  (xi−yi)^2 / (xi+yi)
     * </pre>
     * @param expected
     * @param observed
     * @return
     */
    double scoreChiSquared(const Histogram& expected,const Histogram& observed) const;

    /**
     * Perform Kullback Leibler Divergence
     * 
     * @param model
     * @param sample
     * @return
     */
    double scoreKullbackLeiblerDivergence(const Histogram &expected, const Histogram &observed) const;


    /**
     * Cosine similarity metric
     * The cosine similarity of two models will range from 0 to 1 since the angle between the two vectors can't be more than 90 degrees
     * 1 - exactly similar (angle between them is 0)
     * 0 - orthogonal vectors (angle between them is 90)
     *
     * similarity(A, B) = A . B / ||A|| * ||B||
     *
     * @param expected
     * @param observed
     *
     * @return similarity in range 0..1
     */
    double scoreCosineSimilarity(const Histogram& expected,const Histogram& observed) const;
};


#endif //LBP_MATCHER_HISTOGRAMCOMPARISON_H
