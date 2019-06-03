#ifndef LBP_MATCHER_HISTOGRAMCOMPARISON_H
#define LBP_MATCHER_HISTOGRAMCOMPARISON_H

#include <math.h>
#include <cmath>
#include "types.h"

class HistogramComparison
{
public :
    enum CompareType
    {
        INTERSECTION,
        LOG_LIKELIHOOD,
        CHI_SQUARED,
        KULLBACK_LEIBLER_DIVERGENCE
    };

    /**
     * Compare two histograms
     *
     * @param expected the model we comparing to
     * @param observed the sample we comparing agains
     * @param type Type of comparsion
     * @return score in range 0..1, 0 = No similarity, 1 = Identical
     */
    double compare(const LBPModel& expected, const LBPModel& observed, CompareType type) const;

private :

    /**
     * Perform histogram intersection
     *
     * @param model
     * @param sample
     * @return
     */
    double scoreHistogramIntersection(const LBPModel& expected,const LBPModel& observed) const;

    /**
     * Perform log likehood scoring
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
};


#endif //LBP_MATCHER_HISTOGRAMCOMPARISON_H
