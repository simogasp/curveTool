#pragma once

#include <glm/glm.hpp>

#include <optional>
#include <vector>

using Point = glm::dvec2;

/**
 * Linear interpolate between two points
 * @param p1 The first point
 * @param p2 The second point
 * @param t The weight of the second point
 * @return The created point
 */
Point lerp(const Point& p1, const Point& p2, double t);

std::optional<std::size_t> getClosestPointIndex(const std::vector<Point>& vec, const Point &p, double threshold);
std::optional<Point> getClosestPoint(const std::vector<Point>& vec, const Point &p, double threshold);
bool deletePoint(std::vector<Point>& vec, const Point& p, double threshold);
bool updatePoint(std::vector<Point>& vec, const Point& p_old, const Point& p_new, double threshold);
