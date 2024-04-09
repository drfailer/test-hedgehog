#include <iostream>
#include <memory>
#include <hedgehog/hedgehog.h>
#include <vector>

#include "data/grid.h"
#include "data/grid_block.h"
#include "graph/gol_graph.h"
#include "task/split_grid_task.h"
#include "task/update_block_task.h"
#include "state/update_grid_state.h"

using GridType = bool;

double avgTimers(std::vector<long> const &timers) {
    return (double) std::accumulate(timers.cbegin(), timers.cend(), (long) 0) / (double) timers.size();
}

double stdevTimers(std::vector<long> const &timers) {
    double avg = avgTimers(timers),
                 temp = 0;

    for (auto const &timer : timers) {
        temp += (((double) timer - avg) * ((double) timer - avg));
    }
    return std::sqrt(temp / (double) timers.size());
}

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

int main(int, char**) {
    constexpr size_t gridHeight = 10;
    constexpr size_t gridWidth = 10;
    constexpr size_t blockSize = 3;
    constexpr size_t nbThreads = 3;
    constexpr size_t nbIteration = 5;
    constexpr bool print = true;
    std::vector<long> timers;

    GridType *gridMem = new GridType[gridHeight * gridWidth]();
    GridType *resultMem = new GridType[gridHeight * gridWidth]();

    // add some cells in the grid
    setupGrid(gridMem);

    auto grid = std::make_shared<Grid<GridType>>(gridWidth, gridHeight, blockSize, gridMem);
    auto result = std::make_shared<Grid<GridType>>(gridWidth, gridHeight, blockSize, resultMem);

    std::cout << "start:" << std::endl;
    std::cout << *grid << std::endl;

    GOLGraph<GridType> graph(grid, result, nbThreads);

    graph.executeGraph();
    for (size_t iteration = 0; iteration < nbIteration; ++iteration) {
        std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now(), end;

        // run the simulation for 1 iteration
        graph.pushData(grid);
        *(std::get<std::shared_ptr<Grid<GridType>>>(*graph.getBlockingResult()));

        // print the current grid if required
        if (print) {
            std::cout << *result << std::endl;
        }

        // clean
        graph.cleanGraph();

        // update the timers
        end = std::chrono::system_clock::now();
        timers.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }
    graph.finishPushingData();
    graph.waitForTermination();

    std::cout << "last iteration:" << std::endl;
    std::cout << *grid << std::endl;

    std::cout << "grid: " << gridWidth << "x" << gridHeight << std::endl
              << "block size: " << blockSize << std::endl
              << "avg timer: " << avgTimers(timers) << std::endl
              << "stdev timer: " << stdevTimers(timers) << std::endl
              << "avg timer / size: " << (avgTimers(timers) / static_cast<double>(gridWidth * gridHeight)) << "\n";

    delete[] gridMem;
    delete[] resultMem;
    return 0;
}
