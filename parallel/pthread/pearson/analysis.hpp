/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "vector.hpp"
#include <vector>

#if !defined(ANALYSIS_HPP)
#define ANALYSIS_HPP

namespace Analysis {

std::vector<double>
parallel_correlation_coefficients(std::vector<Vector> &datasets,
                                  int thread_count);
void *correlation_coefficients(void *args);
double pearson(Vector &vec1, Vector &vec2, double x_mean, double y_mean,
               double xmm_over, double ymm_over);
void *pre_compute(void *args);

int get_boundry(std::size_t targetPairs, std::size_t n);
} // namespace Analysis
#endif
