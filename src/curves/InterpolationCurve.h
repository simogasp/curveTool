#pragma once

#include "Point.h"
#include "ControlPoints.h"

#include <vector>
#include <optional>

class InterpolationCurve : public ControlPoints
{
public:
    struct Parameters
    {
        Parameters() = default;
        Parameters(double x_min, double x_max, double p_step) : xmin(x_min), xmax(x_max), step(p_step) { }
        double xmin{0};
        double xmax{100};
        double step{0.1};
    };

    explicit InterpolationCurve() = default;
    explicit InterpolationCurve(const Parameters& p) : param(p) { }
    ~InterpolationCurve() override = default;

    void add(Point p) override;

    [[nodiscard]]
    std::optional<Point> getClosestPoint(const Point& p, double threshold) const override;

    bool deleteControlPoint(const Point& p, double threshold) override;

    bool updateControlPoint(const Point& p_old, const Point& p_new, double threshold) override;
    void updateControlPointAtIndex(std::size_t idx, const Point& p_new, double threshold) override;

    void reset() override;

    [[nodiscard]] const auto& getFunctionalCurve() const { return functionalCurve;}
    [[nodiscard]] const auto& getUniformCurve() const { return uniformCurve;}
    [[nodiscard]] const auto& getDistanceCurve() const { return distanceCurve;}
    [[nodiscard]] const auto& getRootDistanceCurve() const { return rootDistanceCurve;}
    [[nodiscard]] const auto& getChebycheffCurve() const { return chebycheffCurve;}

private:
    void make();

    void makeFunctional();
    void makeUniform();
    void makeDistance();
    void makeRootDistance();
    void makeChebycheff();

    void clearCurves();

    std::vector<Point> functionalCurve{};
    std::vector<Point> uniformCurve{};
    std::vector<Point> distanceCurve{};
    std::vector<Point> rootDistanceCurve{};
    std::vector<Point> chebycheffCurve{};
    Parameters param{};
};


