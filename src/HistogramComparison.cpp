#include "HistogramComparison.h"
#include <iostream>

double clip(double val)
{
    return round(std::min(1.0, std::max(val, 0.0)), 5);
}

double HistogramComparison::compare(const Histogram &expected, const Histogram &observed,
                                    HistogramComparison::CompareType type) const
{
    // make sure both histograms are normalized, i.e. their entries sum up to one
    if(!expected.isNormalized() || !observed.isNormalized())
        throw std::runtime_error("expected and observer histograms need to be normalized before they can be compared");

    switch (type)
    {
        case INTERSECTION :
            return clip(scoreHistogramIntersection(expected, observed));
        case LOG_LIKELIHOOD:
            return clip(scoreLogLikelihood(expected, observed));
        case CHI_SQUARED:
            return clip(scoreChiSquared(expected, observed));
        case KULLBACK_LEIBLER_DIVERGENCE:
            return clip(scoreKullbackLeiblerDivergence(expected, observed));
        case EUCLIDEAN_DISTANCE_NORMALIZED:
            return clip(scoreEuclideanDistanceNormalized(expected, observed));
        case ABSOLUTE_VALUE:
            return clip(scoreAbsoluteValueDistance(expected, observed));
        case COSINE_SIMILARITY:
            return clip(scoreCosineSimilarity(expected, observed));
    }

    throw "Compare type not handled";
}

double HistogramComparison::compare(const Histogram &expected, const Histogram &observed) const
{
    return compare(expected, observed, CompareType::COSINE_SIMILARITY);
}

double HistogramComparison::scoreHistogramIntersection(const Histogram &expected, const Histogram &observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    double d = 0, s1 = 0, s2 = 0;
    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        d  += expected[i] + observed[i] - std::abs(expected[i] - observed[i]);
        s1 += expected[i];
        s2 += observed[i];
    }

    // normalize [0..1]
    return (0.5 * d) / std::fmax(s1, s2);
}

double HistogramComparison::scoreLogLikelihood(const Histogram &expected, const Histogram &observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    double d = 0;
    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        if (expected[i] > 0)
        {
            d += observed[i] * std::log(expected[i]);
        }
    }
    return -d;
}

double HistogramComparison::scoreChiSquared(const Histogram &expected, const Histogram &observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");


    auto d = .0;
    auto bins = expected.size();

    for (int_t i = 0, s = bins; i < s; ++i)
    {
        double q = expected[i] + observed[i];

        if (q != 0)
        {
            double d1 = std::pow(expected[i] - observed[i], 2);
            d += d1 / q;
        }
    }

    // normalize in 1..0 range, 1 = full similarity
    return 1.0 - d;
}


// Kullback-Leibler divergence and Jeffrey divergence
double HistogramComparison::scoreKullbackLeiblerDivergence(const Histogram &model, const Histogram &sample) const
{
    double d = 0;

    for (int_t i = 0, s = model.size(); i < s; ++i)
    {
        double p = model[i];
        double q = sample[i];
        double m = (p + q) / 2;

        //std::cout<<"\n m " << m <<" p " << p <<" q: " << q;

        if(m != 0)
        {
            double h = p * log(p / m);
            double k = q * log(q / m);

            if(!std::isnan(h))
                d += h;

            if(!std::isnan(k))
                d += k;

//            std::cout<<"\n m " << m <<" p " << p <<" q: " << q << " h : " << h<< " k : " << k << " d = "<< d << std::endl;
        }
    }
    return d;
}

double HistogramComparison::scoreEuclideanDistanceNormalized(const Histogram& expected, const Histogram& observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    if(expected.size() == 0)
        throw std::runtime_error("expected or observed size is zero");

    double sum = 0;
    int n = expected.size();
    for (int_t i = 0, s = n; i < s; ++i)
    {
        sum += std::pow(expected[i] - observed[i], 2);
    }

    return std::sqrt(sum / n);
}


double HistogramComparison::scoreAbsoluteValueDistance(const Histogram& expected, const Histogram& observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    if(expected.size() == 0)
        throw std::runtime_error("expected or observed size is zero");

    double sum = 0;
    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        sum += std::abs(expected[i] - observed[i]);
    }

    return clip(1.0 - sum);
}

double HistogramComparison::scoreCosineSimilarity(const Histogram& expected, const Histogram& observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    if(expected.size() == 0)
        throw std::runtime_error("expected or observed size is zero");

    auto dotProduct = .0;
    auto expectedNorm = .0;
    auto observedNorm = .0;

    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        auto v1 = expected[i];
        auto v2 = observed[i];
        dotProduct += v1 * v2;
        expectedNorm += v1 * v1;
        observedNorm += v2 * v2;
    }

    if(expectedNorm == 0 || observedNorm == 0)
        return 0.0;

    return dotProduct / (std::sqrt(expectedNorm) * std::sqrt(observedNorm));
}