#pragma once

#include "Point.h"
#include <vector>

/**
 * @brief Computes the value in x of the Lagrange polynomial passing through the given set of point coordinates.
 * @param[in] x The x coordinate of the point to compute.
 * @param[in] X The list of x coordinates of the points.
 * @param[in] Y The list of y coordinates of the points.
 * @return the value in x of the Lagrange polynomial.
 */
double lagrange(double x, const std::vector<double>& X, const std::vector<double>& Y);

/**
 * @brief Computes the value in x of the Lagrange polynomial passing through the given set of points.
 * @param[in] x The x coordinate of the point to compute.
 * @param[in] points The list of points.
 * @return the value in x of the Lagrange polynomial.
 */
double lagrange(double x, const std::vector<Point>& points);

std::vector<Point> applyLagrangeSubdivision(const std::vector<double>& X,
                                            const std::vector<double>& Y,
                                            const std::vector<double>& T,
                                            const std::vector<double>& tToEval);

std::vector<Point> applyLagrangeSubdivision(const std::vector<Point>& points,
                                            const std::vector<double>& T,
                                            const std::vector<double>& tToEval);