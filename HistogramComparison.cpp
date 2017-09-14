#include "HistogramComparison.h"

double HistogramComparison::compare(const LBPModel &model, const LBPModel &sample,
                                    HistogramComparison::CompareType type) const
{
    switch (type)
    {
        case CompareType::INTERSECTION :
            return scoreHistogramIntersection(model, sample);
        case CompareType::LOG_LIKEHOOD:
            return scoreLogLikelihood(model, sample);
        case CompareType::CHI_SQUARED:
            return scoreChiSquared(model, sample);
        case CompareType::KULLBACK_LEIBLER_DIVERGENCE:
            return scoreKullbackLeiblerDivergence(model, sample);
    }

    throw "Compare type not handled";
}

double HistogramComparison::scoreHistogramIntersection(const LBPModel &model, const LBPModel &sample) const
{
    double d = 0;
    // branch less execution
    for (int_t i = 0; i < model.bin_size; ++i)
    {
        d += model.bins[i] + sample.bins[i] - std::abs(model.bins[i] - sample.bins[i]);
    }

    return 0.5 * d;
}

double HistogramComparison::scoreLogLikelihood(const LBPModel &model, const LBPModel &sample) const
{
    double d = 0;
    for (int_t i = 0; i < model.bin_size; ++i)
    {
        if (model.bins[i] > 0)
        {
            d += sample.bins[i] * std::log(model.bins[i]);
        }
    }
    return -d;
}

double HistogramComparison::scoreChiSquared(const LBPModel &model, const LBPModel &sample) const
{
    double d = 0;
    for (int_t i = 0; i < model.bin_size; ++i)
    {
        double q = sample.bins[i] + model.bins[i];
        if (q != 0)
        {
            double d1 = std::pow(sample.bins[i] - model.bins[i], 2);
            d += d1 / q;
        }
    }
    return d;
}

double HistogramComparison::scoreKullbackLeiblerDivergence(const LBPModel &model, const LBPModel &sample) const
{
    double d = 0;
    for (int_t i = 0; i < model.bin_size; ++i)
    {
        double p = model.bins[i];
        double q = sample.bins[i];

        if (p != 0 && q != 0)
        {
            d += p * std::log(p / q);
        }
    }
    return d;
}
