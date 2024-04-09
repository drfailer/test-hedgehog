#ifndef UPDATE_GRID_STATE_HPP
#define UPDATE_GRID_STATE_HPP

#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/grid.h"
#include "../data/grid_block.h"

template <typename Type>
class UpdateGridState: public hh::AbstractState<1, GridBlock<Type>, Grid<Type>> {
  public:
    UpdateGridState(size_t nbBlocks, std::shared_ptr<Grid<Type>> grid):
        nbTreatedBlock_(nbBlocks), result_(grid) {}

    void execute(std::shared_ptr<GridBlock<Type>>) override {
        --nbTreatedBlock_;
        if (nbTreatedBlock_ == 0) {
            this->addResult(result_);
        }
    }

    void clean() override {
        nbTreatedBlock_ = 0;
    }

    void nbTreatedBlock(size_t nbTreatedBlock) {
        this->nbTreatedBlock_ = nbTreatedBlock;
    }

  private:
    size_t nbTreatedBlock_ = 0;
    std::shared_ptr<Grid<Type>> result_ = nullptr;
};

#endif
