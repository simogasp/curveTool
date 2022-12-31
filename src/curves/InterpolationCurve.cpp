#include "InterpolationCurve.h"

#include "interpolation.h"
#include "parametrization.h"

void InterpolationCurve::add(Point p)
{
    ControlPoints::add(p);
    if(size() > 1)
    {
        make();
    }
}

void InterpolationCurve::make()
{
    makeFunctional();
    makeUniform();
    makeDistance();
    makeRootDistance();
    makeChebycheff();
}
void InterpolationCurve::makeFunctional()
{
    functionalCurve.clear();
    functionalCurve.reserve(static_cast<std::size_t>(std::fabs(param.xmax - param.xmin) / param.step));
    auto xcurr{param.xmin};
    while(xcurr <= param.xmax)
    {
        functionalCurve.emplace_back(xcurr, lagrange(xcurr, getControlPoints()));
        xcurr += param.step;
    }
}

bool InterpolationCurve::updateControlPoint(const Point& p_old, const Point& p_new, double threshold)
{
    if(ControlPoints::updateControlPoint(p_old, p_new, threshold))
    {
        make();
        return true;
    }
    return false;
}
std::optional<Point> InterpolationCurve::getClosestPoint(const Point& p, double threshold) const
{
    return ControlPoints::getClosestPoint(p, threshold);
}
bool InterpolationCurve::deleteControlPoint(const Point& p, double threshold)
{
    if(ControlPoints::deleteControlPoint(p, threshold))
    {
        if(size() > 1)
        {
            make();
        }
        else
        {
            clearCurves();
        }
        return true;
    }
    return false;
}

void InterpolationCurve::reset()
{
    ControlPoints::reset();
    clearCurves();
}

void InterpolationCurve::makeUniform()
{
    uniformCurve.clear();
    const auto [T, tToEval] = uniformSubdivision(size(), param.step);
    uniformCurve = applyLagrangeSubdivision(getControlPoints(), T, tToEval);
}

void InterpolationCurve::makeDistance()
{
    distanceCurve.clear();
    const auto [T, tToEval] = distanceSubdivision(param.step, getControlPoints());
    distanceCurve = applyLagrangeSubdivision(getControlPoints(), T, tToEval);
}

void InterpolationCurve::clearCurves()
{
    functionalCurve.clear();
    uniformCurve.clear();
    distanceCurve.clear();
    rootDistanceCurve.clear();
    chebycheffCurve.clear();
}

void InterpolationCurve::makeRootDistance()
{
    rootDistanceCurve.clear();
    const auto [T, tToEval] = rootDistanceSubdivision(param.step, getControlPoints());
    rootDistanceCurve = applyLagrangeSubdivision(getControlPoints(), T, tToEval);
}

void InterpolationCurve::makeChebycheff()
{
    chebycheffCurve.clear();
    //    const auto [T, tToEval] = chebycheffSubdivision(param.step, getControlPoints());
    const auto [T, tToEval] = chebycheffSubdivision(.01, getControlPoints());
    chebycheffCurve = applyLagrangeSubdivision(getControlPoints(), T, tToEval);
}
