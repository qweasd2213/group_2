CXX = g++
CXXFLAGS = -std=c++17 -g -Wunused -Wall -O3 -flto=auto -march=native -ffp-contract=off -pthread

PAR_DIR = blur_par_pthreads
SEQ_DIR = blur_original

OG_FLAGS = -std=c++17 -g -Wunused -Wall

PAR_SOURCES = $(PAR_DIR)/blur.cpp $(PAR_DIR)/matrix.cpp $(PAR_DIR)/ppm.cpp $(PAR_DIR)/filters.cpp
SEQ_SOURCES = $(SEQ_DIR)/blur.cpp $(SEQ_DIR)/matrix.cpp $(SEQ_DIR)/ppm.cpp $(SEQ_DIR)/filters.cpp

PAR_HEADERS = $(wildcard $(PAR_DIR)/*.hpp)
SEQ_HEADERS = $(wildcard $(SEQ_DIR)/*.hpp)

.PHONY: all clean

all: blur_par blur

blur_par: $(PAR_SOURCES) $(PAR_HEADERS)
	$(CXX) $(CXXFLAGS) $(PAR_SOURCES) -o $@

blur: $(SEQ_SOURCES) $(SEQ_HEADERS)
	$(CXX) $(OG_FLAGS) $(SEQ_SOURCES) -o $@

clean:
	rm -f blur_par blur *.ppm *.o