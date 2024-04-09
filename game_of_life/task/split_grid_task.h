#ifndef SPLIT_GRID_TASK_HPP
#define SPLIT_GRID_TASK_HPP

#include "../data/grid.h"
#include "../data/grid_block.h"
#include <hedgehog/hedgehog.h>

template <typename Type>
class SplitGridTask: public hh::AbstractTask<1, Grid<Type>, GridBlock<Type>> {
  public:
    SplitGridTask(): hh::AbstractTask<1, Grid<Type>, GridBlock<Type>>("Split grid task") {}

    void execute(std::shared_ptr<Grid<Type>> grid) override {
        for (size_t i = 0; i < grid->nbRowBlocks(); ++i) {
            for (size_t j = 0; j < grid->nbColBlocks(); ++j) {
                size_t iGrid = i * grid->blockSize();
                size_t jGrid = j * grid->blockSize();
                this->addResult(std::make_shared<GridBlock<Type>>(
                            grid->blockSize(),
                            grid->width(),
                            grid->height(),
                            i,
                            j,
                            grid->grid()
                            ));
            }
        }
    }
};

#endif
