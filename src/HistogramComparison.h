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
     * Similarity are measured in the range 0 to 1 [0,1]
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
    double compare(const LBPModel& expected, const LBPModel& observed, CompareType type) const;

private :

    /**
     * Euclidean distance
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreEuclideanDistance(const LBPModel& expected, const LBPModel& observed) const;

    /**
     * Euclidian distance normalized
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreEuclideanDistanceNormalized(const LBPModel& expected,const LBPModel& observed) const;

    /**
     * Euclidian distance normalized
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreAbsoluteValueDistance(const LBPModel& expected,const LBPModel& observed) const;

    /**
     * Perform histogram intersection, value will be normalized in [0..1] range
     *
     * @param model
     * @param sample
     * @return
     */
    double scoreHistogramIntersection(const LBPModel& expected,const LBPModel& observed) const;

    /**
     * Perform log likehood scoring
     *
     * @param model
     * @param sample
     * @return
     */
    double scoreLogLikelihood(const LBPModel &expected, const LBPModel &observed) const;

    /**
     * Perform chi-squared goodness of fit test
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreChiSquared(const LBPModel& expected,const LBPModel& observed) const;

    /**
     * Perform Kullback Leibler Divergence
     * @param model
     * @param sample
     * @return
     */
    double scoreKullbackLeiblerDivergence(const LBPModel &expected, const LBPModel &observed) const;


    /**
     * Cosine similarity metric
     *
     * @param expected
     * @param observed
     * @return
     */
    double scoreCosineSimilarity(const LBPModel& expected,const LBPModel& observed) const;
};


#endif //LBP_MATCHER_HISTOGRAMCOMPARISON_H
