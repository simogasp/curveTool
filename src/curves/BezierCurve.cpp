#include "BezierCurve.h"

#include "approximation.h"
#include <utility>

BezierCurve::BezierCurve(std::size_t nbSteps)
{
    this->steps = nbSteps;
    curvePoints.reserve(steps + 1);
}

bool BezierCurve::deleteControlPoint(const Point& p, double threshold)
{
    //    if(::deletePoint(controlPoints, p, threshold))
    if(ControlPoints::deleteControlPoint(p, threshold))
    {
        make();
        return true;
    }
    return false;
}

bool BezierCurve::updateControlPoint(const Point& p_old, const Point& p_new, double threshold)
{
    //    if(updatePoint(controlPoints, p_old, p_new, threshold))
    if(ControlPoints::updateControlPoint(p_old, p_new, threshold))
    {
        make();
        return true;
    }
    return false;
}

void BezierCurve::updateControlPointAtIndex(std::size_t idx, const Point& p_new, double threshold)
{
    ControlPoints::updateControlPointAtIndex(idx, p_new, threshold);
    make();
}

void BezierCurve::make()
{
    curvePoints.clear();
    double incr = 1.0 / static_cast<double>(steps);
    for(double t = 0.0; t <= 1.0; t += incr) // sampling t at regular intervals
    {
        // performing deCasteljau for each t
        curvePoints.push_back(deCasteljau(0, size() - 1, t));
    }
}

void BezierCurve::makeFromVector(const std::vector<Point>& control_points)
{
    setControlPoints(control_points);
    make();
}

void BezierCurve::add(Point p)
{
    ControlPoints::add(p);
    const double incr = 1.0 / static_cast<double>(steps);
    // if this is the first point, just fill the array with the point
    //    if(controlPoints.size() == 1)
    if(size() == 1)
    {
        for(double t = 0.0; t <= 1.0; t += incr)
        {
            curvePoints.push_back(p);
        }
        return;
    }
    // otherwise, you can use the already drawn curve
    std::size_t i{0};
    for(double t = 0.0; t <= 1.0; t += incr)
    {
        //        curvePoints[i] = lerp(curvePoints[i], deCasteljau(1, controlPoints.size() - 1, t), t);
        curvePoints[i] = lerp(curvePoints[i], deCasteljau(1, size() - 1, t), t);
        ++i;
    }
}

Point BezierCurve::deCasteljau(std::size_t start, std::size_t end, double t) const
{
//    // array storing the intermediate points
//    std::vector<Point> temp(getControlPoints());
//    // lerping n (n - 1) / 2 times
//    for(std::size_t i = end; i > start; --i)
//    {
//        for(std::size_t j = start; j < i; ++j)
//            temp[j] = lerp(temp[j], temp[j + 1], t);
//    }
//    return temp[start];
    return ::deCasteljau(getControlPoints(), start, end, t);
}

void BezierCurve::reset()
{
    //    controlPoints.clear();
    ControlPoints::reset();
    curvePoints.clear();
}

std::optional<Point> BezierCurve::getClosestPoint(const Point& p, double threshold) const
{
    return ControlPoints::getClosestPoint(p, threshold);
}
