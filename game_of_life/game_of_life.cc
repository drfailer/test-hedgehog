#include <iostream>
#include <memory>
#include <hedgehog/hedgehog.h>

#include "data/grid.h"
#include "data/grid_block.h"
#include "task/split_grid_task.h"
#include "task/update_block_task.h"
#include "state/update_grid_state.h"

using GridType = bool;

int main(int, char**) {
    constexpr size_t gridHeight = 10;
    constexpr size_t gridWidth = 10;
    constexpr size_t blockSize = 3;
    constexpr size_t nbThreads = 3;

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
    GOLGraph.pushData(grid);
    GOLGraph.finishPushingData();
    GOLGraph.waitForTermination();

    std::cout << *result << std::endl;

    delete[] gridMem;
    delete[] resultMem;
    return 0;
}
