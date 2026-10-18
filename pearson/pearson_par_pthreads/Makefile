# Author: David Holmqvist <daae19@student.bth.se>

CXX=g++-13
CXXFLAGS=-std=c++17 -O3 -pthread -g -Wunused -Wall -Wunused 

all: pearson verify

pearson: threadpool vector dataset analysis pearson.cpp 
	$(CXX) $(CXXFLAGS) pearson.cpp task.o threadpool.o  vector.o dataset.o analysis.o -o pearson_par

analysis: threadpool vector analysis.hpp analysis.cpp
	$(CXX) $(CXXFLAGS) -c analysis.cpp -o analysis.o

dataset: vector dataset.hpp dataset.cpp
	$(CXX) $(CXXFLAGS) -c dataset.cpp -o dataset.o

vector: vector.hpp vector.cpp
	$(CXX) $(CXXFLAGS) -c vector.cpp -o vector.o
threadpool: task threadpool.hpp threadpool.cpp
	$(CXX) $(CXXFLAGS) -c threadpool.cpp -o threadpool.o
task: task.hpp task.cpp
	$(CXX) $(CXXFLAGS) -c task.cpp -o task.o
verify: verify.c
	$(CC) verify.c -o verify
clean:
	rm -rf verify pearson_par *.o *.dSYM 2> /dev/null
