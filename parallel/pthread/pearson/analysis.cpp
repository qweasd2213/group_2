/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis.hpp"
#include "threadpool.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <pthread.h>
#include <stdexcept>
#include <vector>

namespace Analysis {

struct PearsonData {
    PearsonData(std::vector<Vector> &subsection, std::vector<double> &means,
                std::vector<double> &mm_over, int start, int end)
        : subsection(subsection), means(means), mm_over(mm_over),
          startIdx(start), endIdx(end) {}
    std::vector<Vector> &subsection;
    std::vector<double> returnVector = {};
    std::vector<double> &means;
    std::vector<double> &mm_over;
    int startIdx;
    int endIdx;
};
struct PreComputeData {
    PreComputeData(std::vector<Vector> &data, std::vector<double> &means,
                   std::vector<double> &mm_over, int startIdx, int endIdx)
        : data(data), means(means), mm_over(mm_over), startIdx(startIdx),
          endIdx(endIdx) {}
    std::vector<Vector> &data;
    std::vector<double> &means;
    std::vector<double> &mm_over;
    int startIdx;
    int endIdx;
};

int get_boundary(int targetPairs, int n) {
    const double N = static_cast<double>(n);
    const double P = static_cast<double>(targetPairs);

    return static_cast<int>(
        std::ceil(N - 0.5 - std::sqrt(N * N - N + 0.25 - 2.0 * P)));
}
void *pre_compute(void *args) {
    auto *precomputeData = static_cast<PreComputeData *>(args);
    auto &dataset = precomputeData->data;
    auto start = precomputeData->startIdx;
    auto end = precomputeData->endIdx;
    // Pre process the mean values. Previously it calculated for each
    // combination.
    for (auto i{start}; i < end; i++) {
        auto data = dataset[i].get_data();
        auto size = dataset[i].get_size();
        precomputeData->means[i] = dataset[i].mean();
        double sum = 0.0;
        for (auto j{0}; j < size; j++) {
            double deriviation = data[j] - precomputeData->means[i];
            sum += deriviation * deriviation;
        }
        precomputeData->mm_over[i] = sum;
    }
    return nullptr;
}

void *correlation_coefficients(void *args) {
    auto *data = static_cast<PearsonData *>(args);
    int dataSize = data->subsection.size();
    for (auto sample1{data->startIdx}; sample1 < data->endIdx; sample1++) {
        for (auto sample2{sample1 + 1}; sample2 < dataSize; sample2++) {
            auto corr{pearson(data->subsection[sample1],
                              data->subsection[sample2], data->means[sample1],
                              data->means[sample2], data->mm_over[sample1],
                              data->mm_over[sample2])};
            data->returnVector.push_back(corr);
        }
    }
    return nullptr;
}

double pearson(Vector &vec1, Vector &vec2, double x_mean, double y_mean,
               double x_mm_over_x, double y_mm_over_y) {

    // Pre fetch data to remove override indexing calls.
    auto *x = vec1.get_data();
    auto *y = vec2.get_data();
    double convSum = 0.0;
    int size = vec1.get_size();

    for (auto i{0}; i < size; i++) {
        convSum += (x[i] - x_mean) * (y[i] - y_mean);
    }

    const double denom = std::sqrt(x_mm_over_x * y_mm_over_y);

    const double r = convSum / denom;

    return std::max(std::min(r, 1.0), -1.0);
}

std::vector<double>
parallel_correlation_coefficients(std::vector<Vector> &datasets,
                                  int thread_count) {
    if (thread_count <= 0) {
        throw std::invalid_argument("Thread count must be more then 0!");
    }
    Threads::ThreadPool pool(thread_count);

    int dataSize = datasets.size();
    std::vector<PreComputeData> preData;
    preData.reserve(thread_count);

    std::vector<double> means(dataSize);
    std::vector<double> mm_over(dataSize);
    // Split work into chunks
    const int chunkSize = (dataSize + thread_count - 1) / thread_count;
    for (auto i{0}; i < thread_count; i++) {
        auto start = i * chunkSize / thread_count;
        auto end = dataSize * (i + 1) / thread_count;
        preData.push_back(PreComputeData(datasets, means, mm_over, start, end));
        pool.submit(pre_compute, (void *)&preData[i]);
    }
    // Barrier
    pool.wait();
    std::vector<PearsonData> pds;
    pds.reserve(dataSize > 1 ? dataSize - 1 : 0);
    int totalPairs = dataSize * (dataSize - 1) / 2;
    for (auto i{0}; i < thread_count; i++) {
        auto start = get_boundary(totalPairs * i / thread_count, dataSize);
        auto end = get_boundary(totalPairs * (i + 1) / thread_count, dataSize);
        pds.emplace_back(datasets, means, mm_over, start, end);
        pds.back().returnVector.reserve(end - start);
        pool.submit(correlation_coefficients, (void *)&pds[i]);
    }
    // Wait
    pool.wait();
    std::vector<double> result;
    result.reserve(static_cast<int>(dataSize) * (dataSize - 1) / 2);
    for (auto i{0}; i < thread_count; i++) {
        auto &partialResult = pds[i].returnVector;
        result.insert(result.end(), partialResult.begin(), partialResult.end());
    }
    return result;
}
} // namespace Analysis
