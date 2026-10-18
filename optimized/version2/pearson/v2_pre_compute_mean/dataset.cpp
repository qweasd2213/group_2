/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "dataset.hpp"
#include "vector.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>

namespace Dataset {
std::vector<Vector> read(const std::string &filename) {
    unsigned dimension{};
    std::vector<Vector> result{};
    std::ifstream f{};

    f.open(filename);

    if (!f) {
        std::cerr << "Failed to read dataset(s) from file " << filename
                  << std::endl;
        return result;
    }

    f >> dimension;
    std::string line{};

    std::getline(f, line); // ignore first newline

    while (std::getline(f, line)) {
        std::stringstream ss{line};
        Vector new_vec{dimension};
        std::copy_n(std::istream_iterator<double>{ss}, dimension,
                    new_vec.get_data());
        result.push_back(new_vec);
    }

    return result;
}

void write(const std::vector<double> &data, const std::string &filename) {
    std::ofstream f{};

    f.open(filename);

    if (!f) {
        std::cerr << "Failed to write data to file " << filename << std::endl;
        return;
    }
    int size = data.size();
    for (auto i{0}; i < size; i++) {
        f << std::setprecision(std::numeric_limits<double>::digits10 + 1)
          << data[i] << std::endl;
    }
}

}; // namespace Dataset
