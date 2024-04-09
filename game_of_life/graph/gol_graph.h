#ifndef GOL_GRAPH_HPP
#define GOL_GRAPH_HPP

#include <hedgehog/hedgehog.h>
#include "../data/grid.h"
#include "../data/grid_block.h"
#include "../task/split_grid_task.h"
#include "../task/update_block_task.h"
#include "../state/update_grid_state.h"

template <typename Type>
class GOLGraph: public hh::Graph<1, Grid<Type>, Grid<Type>> {
  public:
    GOLGraph(std::shared_ptr<Grid<Type>> grid, std::shared_ptr<Grid<Type>> result, size_t nbThreads):
        hh::Graph<1, Grid<Type>, Grid<Type>>() {
            auto splitGridTask = std::make_shared<SplitGridTask<Type>>();
            auto updateBlockTask = std::make_shared<UpdateBlockTask<Type>>(nbThreads, result);
            auto updateGridState = std::make_shared<
                UpdateGridState<Type>>(grid->nbRowBlocks() * grid->nbColBlocks(), grid, result);
            auto updateSateManager = std::make_shared<
                hh::StateManager<1, GridBlock<Type>, Grid<Type>>>(updateGridState, "Update Grid");

            this->inputs(splitGridTask);
            this->edges(splitGridTask, updateBlockTask);
            this->edges(updateBlockTask, updateSateManager);
            this->outputs(updateSateManager);
        }
};

#endif
