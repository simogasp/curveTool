#pragma once

#include "Point.h"
#include <vector>

/**
 *
 * @param start
 * @param end
 * @param t
 * @return
 */
Point deCasteljau(std::vector<Point> controlPoints, std::size_t start, std::size_t end, double t);