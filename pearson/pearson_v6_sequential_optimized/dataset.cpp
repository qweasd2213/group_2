/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "dataset.hpp"
#include "vector.hpp"
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
namespace Dataset {
std::vector<Vector> read(const std::string &filename) {
    // Ensure empty result.
    unsigned dimension{};
    std::ifstream f{};
    std::vector<Vector> result;
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
        const char *currentPos = line.data();
        const char *endPos = currentPos + line.size();
        Vector new_vec{dimension};
        double *data = new_vec.get_data();
        unsigned i = 0;
        while (i < dimension) {
            // Skipping any blank spaces.
            auto [nextPos, ec] = std::from_chars(currentPos, endPos, data[i]);

            currentPos = nextPos;
            // Skipping the guaranteed whitespaces.
            if (currentPos != endPos) {
                ++currentPos;
            }
            ++i;
        }
        result.push_back(std::move(new_vec));
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

    int precision = std::numeric_limits<double>::digits10 + 1;
    const int charSizeOfDouble = 128;
    char tmpBuf[charSizeOfDouble];
    // Pre convert doubles to char, using to_chars instead of normal >>
    // operator.
    for (auto value : data) {
        auto convertedValue =
            std::to_chars(tmpBuf, tmpBuf + charSizeOfDouble, value,
                          std::chars_format::general, precision);
        f.write(tmpBuf, convertedValue.ptr - tmpBuf);
        f.put('\n');
    }
}

}; // namespace Dataset
