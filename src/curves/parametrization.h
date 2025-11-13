#pragma once

#include "Point.h"
#include <vector>
#include <tuple>

/**
 * @brief Generate a uniform parametrization between 0 and 1.0 for the given step number.
 * @param[in] step The step
 * @return a vector of step+1 elements between 0 and 1 with an increment of 1/step
 */
std::vector<double> uniformParametrization(std::size_t step);



void uniformSubdivision(std::size_t nbElem, double step, std::vector<double>& T, std::vector<double>& tToEval);
std::tuple<std::vector<double>, std::vector<double>> uniformSubdivision(std::size_t nbElem, double pas);

std::tuple<std::vector<double>, std::vector<double>> distanceSubdivision(double pas, const std::vector<Point>& points);
void distanceSubdivision(double step, const std::vector<Point>& points, std::vector<double>& T, std::vector<double>& tToEval);

std::tuple<std::vector<double>, std::vector<double>> rootDistanceSubdivision(double pas, const std::vector<Point>& points);
void rootDistanceSubdivision(double step, const std::vector<Point>& points, std::vector<double>& T, std::vector<double>& tToEval);

std::tuple<std::vector<double>, std::vector<double>> chebycheffSubdivision(double pas, const std::vector<Point>& points);
void chebycheffSubdivision(double step, const std::vector<Point>& points, std::vector<double>& T, std::vector<double>& tToEval);

std::vector<double> computeDistanceSubdivision(const std::vector<Point>& points);
std::vector<double> computeRootDistanceSubdivision(const std::vector<Point>& points);
std::vector<double> computeChebycheffSubdivision(const std::vector<Point>& points);

std::vector<double> createSamples(double step, const std::vector<double>& T);