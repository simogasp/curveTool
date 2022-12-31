#include "interpolation.h"


double lagrange(double x, const std::vector<double>& X, const std::vector<double>& Y)
{
    assert(X.size() == Y.size());
    const auto numPts = X.size();

    double sum{0};
    for(std::size_t i = 0; i < numPts; ++i)
    {
        double value{1};
        for(std::size_t j = 0; j < numPts; ++j)
        {
            if(i != j)
            {
                value = value * (x - X[j]) / (X[i] - X[j]);
            }
        }
        sum = sum + Y[i] * value;
    }
    return sum;
}

std::tuple<std::vector<double>, std::vector<double>> splitCoordinates(const std::vector<Point>& points)
{
    std::vector<double> X{};
    X.reserve(points.size());
    std::vector<double> Y{};
    Y.reserve(points.size());
    for(const auto& p : points)
    {
        X.push_back(p.x);
        Y.push_back(p.y);
    }
    return {X, Y};
}

double lagrange(double x, const std::vector<Point>& points)
{
    const auto[X, Y] = splitCoordinates(points);
    return lagrange(x, X, Y);
}

std::vector<Point> applyLagrangeSubdivision(const std::vector<Point>& points,
                                            const std::vector<double>& T,
                                            const std::vector<double>& tToEval)
{
    const auto[X, Y] = splitCoordinates(points);
    return applyLagrangeSubdivision(X, Y, T, tToEval);
}

std::vector<Point> applyLagrangeSubdivision(const std::vector<double>& X,
                                            const std::vector<double>& Y,
                                            const std::vector<double>& T,
                                            const std::vector<double>& tToEval)
{
    const auto numSamples{tToEval.size()};
    std::vector<Point> curve{};
    curve.reserve(numSamples);
    for (auto t : tToEval)
    {
        const auto xpoint = lagrange(t, T, X);
        const auto ypoint = lagrange(t, T, Y);
        curve.emplace_back(xpoint, ypoint);
    }
    return curve;
}