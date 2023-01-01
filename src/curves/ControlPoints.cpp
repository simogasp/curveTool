#include "ControlPoints.h"

bool ControlPoints::deleteControlPoint(const Point& p, double threshold)
{
    return deletePoint(controlPoints, p, threshold);
}

bool ControlPoints::updateControlPoint(const Point& p_old, const Point& p_new, double threshold)
{
    return updatePoint(controlPoints, p_old, p_new, threshold);
}

void ControlPoints::updateControlPointAtIndex(std::size_t idx, const Point& p_new, double threshold)
{
    return updatePointAtIndex(controlPoints, idx, p_new);
}

void ControlPoints::add(Point p)
{
    controlPoints.push_back(p);
}

void ControlPoints::reset()
{
    controlPoints.clear();
}

std::optional<Point> ControlPoints::getClosestPoint(const Point& p, double threshold) const
{
    return ::getClosestPoint(controlPoints, p, threshold);
}

std::optional<std::size_t> ControlPoints::getIndexClosestPoint(const Point& p, double threshold) const
{
    return ::getClosestPointIndex(controlPoints, p, threshold);
}

void ControlPoints::setControlPoints(const std::vector<Point>& ctrlPoints)
{
    this->controlPoints = ctrlPoints;
}
