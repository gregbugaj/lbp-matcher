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
        LOG_LIKEHOOD,
        CHI_SQUARED,
        KULLBACK_LEIBLER_DIVERGENCE
    };

    /**
     * Compare two histograms
     * @param model the model we comparing to
     * @param sample the sample we comparing agains
     * @param type Type of comparsion
     * @return score in range 0..1, 0 = No similarity, 1 = Identical
     */
    double compare(const LBPModel&  model, const LBPModel&  sample, CompareType type) const;

private :
    /**
     * Perform histogram intersection
     * @param model
     * @param sample
     * @return
     */
    double scoreHistogramIntersection(const LBPModel& model,const LBPModel& sample) const;

    /**
     * Perform log likehood scoring
     * @param model
     * @param sample
     * @return
     */
    double scoreLogLikehood(const LBPModel& model,const LBPModel& sample) const;

    /**
     * Perform chi-squared scoring
     * @param model
     * @param sample
     * @return
     */
    double scoreChiSquared(const LBPModel& model,const LBPModel& sample) const;

    /**
     * Perform Kullback Leibler Divergence
     * @param model
     * @param sample
     * @return
     */
    double scoreKullbackLeiblerDivergence(const LBPModel &model, const LBPModel &sample) const;
};


#endif //LBP_MATCHER_HISTOGRAMCOMPARISON_H
