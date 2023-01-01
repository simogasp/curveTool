#pragma once

#include "Point.h"
#include "ControlPoints.h"

#include <glm/glm.hpp>

#include <optional>
#include <vector>

class BezierCurve : public ControlPoints
{
public:

    /**
     * The constructor to make a curve drawer.
     * @param Steps The number of steps for the t value
     */
    explicit BezierCurve(std::size_t steps);

    ~BezierCurve() override = default;

    /**
     * Makes a new curve from the given vector of control points
     * @param ControlPoints The control points for the new curve
     */
    void makeFromVector(const std::vector<Point>& ControlPoints);

    /**
     * Adds one point to the control points of the Bezier curve at the end
     * @param p The point to be added
     */
    void add(Point p) override;

    [[nodiscard]]
    std::optional<Point> getClosestPoint(const Point& p, double threshold) const override;

    bool deleteControlPoint(const Point& p, double threshold) override;

    bool updateControlPoint(const Point& p_old, const Point& p_new, double threshold) override;
    void updateControlPointAtIndex(std::size_t idx, const Point& p_new, double threshold) override;

    void reset() override;

    [[nodiscard]] const auto& getCurvePoint() const {return curvePoints;}

private:
    /**
     * Applies the deCasteljau's algorithm between the start and the end index of the controlPoints
     * @param start The starting index
     * @param end The ending index
     * @param t The t value
     * @return The generated point on the curve
     */
    [[nodiscard]] Point deCasteljau(size_t start, size_t end, double t) const;

    /**
     * Makes the curve based on the stored control points.
     */
    void make();

    /// the number of steps for t
    std::size_t steps;
    /// the array of control points
    //std::vector<Point> controlPoints;
    /// the array of the actual curvePoints
    std::vector<Point> curvePoints;
};
