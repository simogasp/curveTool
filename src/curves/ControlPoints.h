#pragma once

#include "Point.h"
#include "Subject.h"

#include <vector>
#include <optional>

class ControlPoints
{
public:
    ControlPoints() = default;
    virtual ~ControlPoints() = default;

    virtual void add(Point p);

    [[nodiscard]] virtual std::optional<Point> getClosestPoint(const Point& p, double threshold) const;
    [[nodiscard]] virtual std::optional<std::size_t> getIndexClosestPoint(const Point& p, double threshold) const;

    virtual bool deleteControlPoint(const Point& p, double threshold);

    virtual bool updateControlPoint(const Point& p_old, const Point& p_new, double threshold);
    virtual void updateControlPointAtIndex(std::size_t idx, const Point& p_new, double threshold);

    virtual void reset();

    [[nodiscard]] virtual const std::vector<Point>& getControlPoints() const { return controlPoints; }

    virtual std::size_t size() { return controlPoints.size(); }

protected:

    void setControlPoints(const std::vector<Point>& ctrlPoints);

//    using iterator = std::vector<Point>::iterator;
//    using const_iterator = std::vector<Point>::const_iterator;
//
//    iterator begin() { return controlPoints.begin(); }
//
//    iterator end() { return controlPoints.end(); }
//
//    [[nodiscard]] const_iterator cbegin() const { return controlPoints.cbegin(); }
//
//    [[nodiscard]] const_iterator cend() const { return controlPoints.cend(); }

private:
    std::vector<Point> controlPoints;
};
