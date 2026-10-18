/*
Author: David Holmqvist <daae19@student.bth.se>
*/

#include "analysis.hpp"
#include "dataset.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " [dataset] [outfile] [thread_count]" << std::endl;
        std::exit(1);
    }
    auto datasets{Dataset::read(argv[1])};
    auto corrs{Analysis::parallel_correlation_coefficients(datasets,
                                                           std::stoi(argv[3]))};
    Dataset::write(corrs, argv[2]);

    return 0;
}
