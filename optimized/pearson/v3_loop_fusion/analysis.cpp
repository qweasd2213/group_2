/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

namespace Analysis {

std::vector<double> correlation_coefficients(std::vector<Vector> &datasets) {
    std::vector<double> result{};
    int datasetSize = datasets.size();
    std::vector<double> means{};
    means.resize(datasetSize);

    // Pre process the mean values. Previously it calculated for each
    // combination.
    for (auto i{0}; i < datasetSize; i++) {
        means[i] = datasets[i].mean();
    }

    for (auto sample1{0}; sample1 < datasetSize - 1; sample1++) {
        for (auto sample2{sample1 + 1}; sample2 < datasetSize; sample2++) {
            auto corr{pearson(datasets[sample1], datasets[sample2],
                              means[sample1], means[sample2])};
            result.push_back(corr);
        }
    }
    return result;
}

double pearson(Vector &vec1, Vector &vec2, double x_mean, double y_mean) {
    double convSum = 0.0;
    double x_mm_over_x = 0.0;
    double y_mm_over_y = 0.0;
    int size = vec1.get_size();
    for (auto i{0}; i < size; i++) {
        const double x_mm = vec1[i] - x_mean;
        const double y_mm = vec2[i] - y_mean;

        convSum += x_mm * y_mm;
        x_mm_over_x += x_mm * x_mm;
        y_mm_over_y += y_mm * y_mm;
    }

    const double denom = std::sqrt(x_mm_over_x * y_mm_over_y);

    const double r = convSum / denom;

    return std::max(std::min(r, 1.0), -1.0);
}
}; // namespace Analysis
