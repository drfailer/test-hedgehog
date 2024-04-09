#include <iostream>
#include <memory>
#include <hedgehog/hedgehog.h>
#include <vector>

#include "data/grid.h"
#include "data/grid_block.h"
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

int main(int, char**) {
    constexpr size_t gridHeight = 100;
    constexpr size_t gridWidth = 100;
    constexpr size_t blockSize = 10;
    constexpr size_t nbThreads = 4;
    constexpr size_t nbIteration = 100;
    constexpr bool print = false;
    std::vector<long> timers;

    GridType *gridMem = new GridType[gridHeight * gridWidth]();
    GridType *resultMem = new GridType[gridHeight * gridWidth]();

    gridMem[0] = true;
    gridMem[1] = true;
    gridMem[2] = true;
    gridMem[10 + 0] = true;
    gridMem[10 + 1] = true;
    gridMem[10 + 2] = true;
    gridMem[20 + 0] = true;
    gridMem[20 + 1] = true;
    gridMem[20 + 2] = true;

    gridMem[4*10 + 4] = true;
    gridMem[4*10 + 5] = true;
    gridMem[4*10 + 6] = true;

    auto grid = std::make_shared<Grid<GridType>>(gridWidth, gridHeight, blockSize, gridMem);
    auto result = std::make_shared<Grid<GridType>>(gridWidth, gridHeight, blockSize, resultMem);

    std::cout << "start:" << std::endl;
    std::cout << *grid << std::endl;

    auto splitGridTask = std::make_shared<SplitGridTask<GridType>>();
    auto updateBlockTask = std::make_shared<UpdateBlockTask<GridType>>(nbThreads, result);
    auto updateGridState = std::make_shared<UpdateGridState<GridType>>(grid->nbRowBlocks() * grid->nbColBlocks(), result);
    auto updateSateManager = std::make_shared<
        hh::StateManager<1, GridBlock<GridType>, Grid<GridType>>>(updateGridState, "Update Grid");
    auto GOLGraph = hh::Graph<1, Grid<GridType>, Grid<GridType>>();

    GOLGraph.inputs(splitGridTask);
    GOLGraph.edges(splitGridTask, updateBlockTask);
    GOLGraph.edges(updateBlockTask, updateSateManager);
    GOLGraph.outputs(updateSateManager);

    GOLGraph.executeGraph();
    for (size_t iteration = 0; iteration < nbIteration; ++iteration) {
        std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now(), end;

        // run the simulation for 1 iteration
        GOLGraph.pushData(grid);
        *(std::get<std::shared_ptr<Grid<GridType>>>(*GOLGraph.getBlockingResult()));

        // print the current grid if required
        if (print) {
            std::cout << *result << std::endl;
        }

        // clean
        GOLGraph.cleanGraph();
        result->swap(grid.get());
        updateGridState->nbTreatedBlock(grid->nbRowBlocks() * grid->nbColBlocks());

        // update the timers
        end = std::chrono::system_clock::now();
        timers.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }
    GOLGraph.finishPushingData();
    GOLGraph.waitForTermination();

    std::cout << "last iteration:" << std::endl;
    std::cout << *result << std::endl;

    std::cout << "grid: " << gridWidth << "x" << gridHeight << std::endl
              << "block size: " << blockSize << std::endl
              << "avg timer: " << avgTimers(timers) << std::endl
              << "stdev timer: " << stdevTimers(timers) << std::endl
              << "avg timer / size: " << (avgTimers(timers) / static_cast<double>(gridWidth * gridHeight)) << "\n";

    delete[] gridMem;
    delete[] resultMem;
    return 0;
}
