

#include "parametrization.h"
#include "Point.h"
#include <numeric>
#include <algorithm>
#include <iostream>
#include <glm/ext/scalar_constants.hpp>

std::vector<double> uniformParametrization(std::size_t step)
{
    const auto increment = (1.0 / (double)step);
    std::vector<double> T;
    T.reserve(step + 1);
    for(std::size_t i = 0; i < step; ++i)
    {
        T.push_back((double)i * increment);
    }
    T.push_back(1.0);
    return T;
}

void uniformSubdivision(std::size_t nbElem, double step, std::vector<double>& T, std::vector<double>& tToEval)
{
    T.resize(nbElem);
    std::iota(T.begin(), T.end(), .0);

    tToEval = createSamples(step, T);
}


std::tuple<std::vector<double>, std::vector<double>> uniformSubdivision(std::size_t nbElem, double pas)
{
    std::vector<double> T;
    std::vector<double> tToEval;
    uniformSubdivision(nbElem, pas, T, tToEval);
    return {T, tToEval};
}


std::tuple<std::vector<double>, std::vector<double>> distanceSubdivision( double pas, const std::vector<Point>& points)
{
    std::vector<double> T;
    std::vector<double> tToEval;
    distanceSubdivision(pas, points, T, tToEval);
    return {T, tToEval};
}

void distanceSubdivision(double step, const std::vector<Point>& points, std::vector<double>& T, std::vector<double>& tToEval)
{
    T = computeDistanceSubdivision(points);

    tToEval = createSamples(step, T);
}

std::tuple<std::vector<double>, std::vector<double>> rootDistanceSubdivision(double pas, const std::vector<Point>& points)
{
    std::vector<double> T;
    std::vector<double> tToEval;
    rootDistanceSubdivision(pas, points, T, tToEval);
    return {T, tToEval};
}

void rootDistanceSubdivision(double step, const std::vector<Point>& points, std::vector<double>& T, std::vector<double>& tToEval)
{
    T = computeRootDistanceSubdivision(points);
    tToEval = createSamples(step, T);
}

std::tuple<std::vector<double>, std::vector<double>> chebycheffSubdivision(double pas, const std::vector<Point>& points)
{
    std::vector<double> T;
    std::vector<double> tToEval;
    chebycheffSubdivision(pas, points, T, tToEval);
    return {T, tToEval};
}

void chebycheffSubdivision(double step, const std::vector<Point>& points, std::vector<double>& T, std::vector<double>& tToEval)
{
    T = computeChebycheffSubdivision(points);
    tToEval = createSamples(step, T);
}


std::vector<double> computeDistanceSubdivision(const std::vector<Point>& points)
{
    const auto nbElem = points.size();
    std::vector<double> T(nbElem);

    T[0] = .0;
    for(std::size_t i = 1; i < nbElem; ++i)
    {
        const auto d = glm::distance(points[i - 1], points[i]);
        T[i] = T[i - 1] + d;
    }
    return T;
}

std::vector<double> computeRootDistanceSubdivision(const std::vector<Point>& points)
{
    const auto nbElem = points.size();
    std::vector<double> T(nbElem);

    T[0] = .0;
    for(std::size_t i = 1; i < nbElem; ++i)
    {
        const auto d = glm::distance(points[i - 1], points[i]);
        T[i] = T[i - 1] + std::sqrt(d);
    }
    return T;
}

std::vector<double> computeChebycheffSubdivision(const std::vector<Point>& points)
{
    const auto nbElem = points.size();
    std::vector<double> T(nbElem);

    for (std::size_t i = 0; i < nbElem; ++i)
    {
        const double v = ((2 * static_cast<double>(i) + 1) * glm::pi<double>()) / static_cast<double>(2 * (nbElem - 1) + 2);
        T[i] = std::cos(v);
    }

    return T;
}

std::vector<double> createSamples(double step, const std::vector<double>& T)
{
    // Start value for the samples
    const auto start = *std::min_element(T.begin(), T.end());
    // End value for the samples
    const auto end =  *std::max_element(T.begin(), T.end());
    const auto numSamples = static_cast<std::size_t>((end-start) / step);
    std::vector<double> tToEval(numSamples+1);

    for(std::size_t i{0}; i < tToEval.size(); ++i)
    {
        tToEval[i] = start + static_cast<double>(i) * step;
    }
    // just assure that the last element is the end value
    if(tToEval.back() < end)
    {
        tToEval.push_back(end);
    }

    return tToEval;
}