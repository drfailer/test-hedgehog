#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <vector>
#include <memory>
#include <thread>
#include <iostream>

#include "data/grid.h"

struct Config {
    size_t gridWidth;
    size_t gridHeight;
    size_t blockSize;
    size_t nbThreads;
    size_t nbEpocs;
    bool animation;
    bool printGrid;
    bool generateGraph;
    double population;
};


double avgTimers(std::vector<long> const &timers);
double stdevTimers(std::vector<long> const &timers);
void parseCmdArgs(int argc, char **argv, Config &config);

template <typename Type>
void animate(std::shared_ptr<Grid<Type>> result) {
    using namespace std::chrono_literals; // for sleep
    system("clear");
    std::cout << *result << std::endl;
    std::this_thread::sleep_for(700ms);
}

#endif
