#include "HistogramComparison.h"
#include <iostream>
#include <iomanip>

/**
 * round value
 *
 * @param val
 * @param precision
 * @return
 */
template <class T>
double round(T val, int precision)
{
    std::stringstream s;
    s << std::setprecision(precision) << std::setiosflags(std::ios_base::fixed) << val;
    s >> val;
    return val;
}

double HistogramComparison::compare(const Histogram &expected, const Histogram &observed,
                                    HistogramComparison::CompareType type) const
{
    switch (type)
    {
        case INTERSECTION :
            return scoreHistogramIntersection(expected, observed);
        case LOG_LIKELIHOOD:
            return scoreLogLikelihood(expected, observed);
        case CHI_SQUARED:
            return scoreChiSquared(expected, observed);
        case KULLBACK_LEIBLER_DIVERGENCE:
            return scoreKullbackLeiblerDivergence(expected, observed);
        case EUCLIDEAN_DISTANCE:
            return scoreEuclideanDistance(expected, observed);
        case EUCLIDEAN_DISTANCE_NORMALIZED:
            return scoreEuclideanDistanceNormalized(expected, observed);
        case ABSOLUTE_VALUE:
            return scoreAbsoluteValueDistance(expected, observed);
        case COSINE_SIMILARITY:
            return scoreCosineSimilarity(expected, observed);
    }

    throw "Compare type not handled";
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
    return round((0.5 * d) / std::fmax(s1, s2), 2);
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

    double d = 0;
    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        double m = observed[i] + expected[i];

        if (m != 0)
        {
            double d1 = std::pow(expected[i] - m, 2);
            d += (d1 / m);
        }
    }
    return d;
}

double scoreChiSquaredGoodnesOfFit(const Histogram &expected, const Histogram &observed)
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    if(expected.size() < 2 || observed.size() < 2)
        throw std::runtime_error("expected or observed size is smaller than 2");

    double d = 0;
    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        // ensure that neither sample is 0
        if(observed[i] == 0 ||  expected[i] == 0)
            return 0;

        double d1 = std::pow(observed[i] - expected[i], 2);
        d += (d1 / expected[i]);
    }
    return d;
}


// Kullback-Leibler divergence and Jefrey divergence
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

            std::cout<<"\n m " << m <<" p " << p <<" q: " << q << " h : " << h<< " k : " << k << " d = "<< d << std::endl;
        }
    }
    return d;
}

double scoreKullbackLeiblerDivergenceORG(const Histogram &model, const Histogram &sample)
{
    double d = 0;
    for (int_t i = 0, s = model.size(); i < s; ++i)
    {
        double p = model[i];
        double q = sample[i];

        if (p != 0 && q != 0)
        {
            d += p * std::log(p / q);
        }
    }
    return d;
}

double HistogramComparison::scoreEuclideanDistance(const Histogram& expected, const Histogram& observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    if(expected.size() == 0)
        throw std::runtime_error("expected or observed size is zero");

    double sum = 0;
    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        sum += std::pow(expected[i] - observed[i], 2);
    }

    return std::sqrt(sum);
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

    return sum;
}

double HistogramComparison::scoreCosineSimilarity(const Histogram& expected, const Histogram& observed) const
{
    if(expected.size() != observed.size())
        throw std::runtime_error("expected size != observed size");

    if(expected.size() == 0)
        throw std::runtime_error("expected or observed size is zero");

    auto dotProduct = 0;
    auto expectedNorm = 0;
    auto observedNorm = 0;

    for (int_t i = 0, s = expected.size(); i < s; ++i)
    {
        auto v1 = expected[i];
        auto v2 = observed[i];
        dotProduct += v1 * v2;
        expectedNorm += v1 * v1;
        observedNorm += v2 * v2;
    }

    if(expectedNorm == 0 || observedNorm == 0)
        return 0;

    return round(dotProduct / (std::sqrt(expectedNorm) * std::sqrt(observedNorm)), 3);
}

double scoreChiSquaredORG(const Histogram &model, const Histogram &sample)
{
    double d = 0;
    for (int_t i = 0, s = model.size(); i < s; ++i)
    {
        double q = sample[i] + model[i];

        if (q != 0)
        {
            double d1 = std::pow(sample[i] - model[i], 2);
            d += d1 / q;
        }
    }
    return d;
}