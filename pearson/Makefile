CXX = g++
CXXFLAGS = -std=c++17 -g -Wunused -Wall -O3

PAR_DIR = pearson_par_pthreads
SEQ_DIR = pearson_v6_sequential_optimized

PAR_FLAGS = $(CXXFLAGS) -pthread

PAR_SOURCES = $(PAR_DIR)/pearson.cpp $(PAR_DIR)/vector.cpp $(PAR_DIR)/dataset.cpp $(PAR_DIR)/analysis.cpp $(PAR_DIR)/threadpool.cpp $(PAR_DIR)/task.cpp
SEQ_SOURCES = $(SEQ_DIR)/pearson.cpp $(SEQ_DIR)/vector.cpp $(SEQ_DIR)/dataset.cpp $(SEQ_DIR)/analysis.cpp
PAR_HEADERS = $(wildcard $(PAR_DIR)/*.hpp)
SEQ_HEADERS = $(wildcard $(SEQ_DIR)/*.hpp)

.PHONY: all clean

all: pearson_par pearson_opt verify

pearson_par: $(PAR_SOURCES) $(PAR_HEADERS)
	$(CXX) $(PAR_FLAGS) $(PAR_SOURCES) -o $@

pearson_opt: $(SEQ_SOURCES) $(SEQ_HEADERS)
	$(CXX) $(CXXFLAGS) $(SEQ_SOURCES) -o $@
verify: verify.c
	$(CC) $(CXXFLAGS) -o verify
clean:
	rm -f pearson_par pearson_opt *.ppm *.o
