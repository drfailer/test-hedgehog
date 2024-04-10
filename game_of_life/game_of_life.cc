#include <cstdlib>
#include <iostream>
#include <memory>
#include <hedgehog/hedgehog.h>
#include <vector>

#include "data/grid.h"
#include "graph/gol_graph.h"
#include "utils.h"

using GridType = bool;

void setupGrid(GridType *grid) {
    grid[0] = true;
    grid[1] = true;
    grid[2] = true;
    grid[10 + 0] = true;
    grid[10 + 1] = true;
    grid[10 + 2] = true;
    grid[20 + 0] = true;
    grid[20 + 1] = true;
    grid[20 + 2] = true;

    grid[4*10 + 4] = true;
    grid[4*10 + 5] = true;
    grid[4*10 + 6] = true;
}

int main(int argc, char** argv) {
    Config config;
    std::vector<long> timers;

    parseCmdArgs(argc, argv, config);

    GridType *gridMem = new GridType[config.gridHeight * config.gridWidth]();
    GridType *resultMem = new GridType[config.gridHeight * config.gridWidth]();
    setupGrid(gridMem);

    auto grid = std::make_shared<Grid<GridType>>(config.gridWidth, config.gridHeight, config.blockSize, gridMem);
    auto result = std::make_shared<Grid<GridType>>(config.gridWidth, config.gridHeight, config.blockSize, resultMem);

    if (config.printGrid) {
        std::cout << "start:" << std::endl;
        std::cout << *grid << std::endl;
    }

    GOLGraph<GridType> graph(grid, result, config.nbThreads);

    graph.executeGraph();
    for (size_t iteration = 0; iteration < config.nbEpocs; ++iteration) {
        std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now(), end;

        graph.pushData(grid);
        *(std::get<std::shared_ptr<Grid<GridType>>>(*graph.getBlockingResult()));
        if (config.animation) { animate(result); } // animate
        graph.cleanGraph();
        if (!config.animation) { // update timers
            end = std::chrono::system_clock::now();
            timers.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
        }

        if (iteration == 0 && config.generateGraph) {
            graph.createDotFile("golGraph.dot",
                    hh::ColorScheme::EXECUTION, hh::StructureOptions::ALL);
        }
    }
    graph.finishPushingData();
    graph.waitForTermination();

    if (config.printGrid) {
        std::cout << "last iteration:" << std::endl;
        std::cout << *grid << std::endl;
    }

    if (!config.animation) { // this is useless in animation mode
        std::cout << "grid: " << config.gridWidth << "x" << config.gridHeight << std::endl
            << "block size: " << config.blockSize << std::endl
            << "avg timer: " << avgTimers(timers) << std::endl
            << "stdev timer: " << stdevTimers(timers) << std::endl
            << "avg timer / size: " << (avgTimers(timers) / static_cast<double>(config.gridWidth * config.gridHeight)) << "\n";
    }

    delete[] gridMem;
    delete[] resultMem;
    return 0;
}
