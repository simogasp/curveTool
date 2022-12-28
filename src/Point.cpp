#include "Point.h"

Point lerp(const Point &p1, const Point &p2, double t)
{
    //linear interpolation
    return (1 - t) * p1 + t * p2;
}

std::optional<std::size_t> getClosestPointIndex(const std::vector<Point>& vec, const Point &p, double threshold)
{
    double closestDistance = std::numeric_limits<double>::max();
    std::size_t idxPoint{0};;
    // Iterate through the points and find the closest point within the threshold
    for(std::size_t i{0}; i < vec.size(); ++i)
    {
        const double dist = distance(vec[i], p);
        if (dist <= threshold && dist < closestDistance)
        {
            idxPoint = i;
            closestDistance = dist;
        }
    }

    // If the closest distance is the maximum possible value, return an empty optional
    if (closestDistance == std::numeric_limits<double>::max())
    {
        return {};
    }

    // Otherwise, return the index of the element containing the closest point and distance
    return {idxPoint};
}

std::optional<Point> getClosestPoint(const std::vector<Point>& vec, const Point &p, double threshold)
{
    const auto res = getClosestPointIndex(vec, p, threshold);
    if(res.has_value())
    {
        return {vec[res.value()]};
    }
    return {};
}

bool deletePoint(std::vector<Point>& vec, const Point& p, double threshold)
{
    const auto res = getClosestPointIndex(vec, p, threshold);
    if(res.has_value())
    {
        vec.erase(std::next(vec.begin(), static_cast<long>(res.value())));
        return true;
    }
    return false;
}

bool updatePoint(std::vector<Point>& vec, const Point& p_old, const Point& p_new, double threshold)
{
    const auto res = getClosestPointIndex(vec, p_old, threshold);
    if(res.has_value())
    {
        vec[res.value()] = p_new;
        return true;
    }
    return false;
}