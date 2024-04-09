#ifndef UPDATE_BLOCK_TASK_HPP
#define UPDATE_BLOCK_TASK_HPP

#include <algorithm>
#include <cstddef>
#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/grid_block.h"
#include "../data/grid.h"

template <typename Type>
class UpdateBlockTask: public hh::AbstractTask<1, GridBlock<Type>, GridBlock<Type>> {
  public:
    UpdateBlockTask(size_t numberThreads, std::shared_ptr<Grid<Type>> result):
        result_(result),
        hh::AbstractTask<1, GridBlock<Type>, GridBlock<Type>>("Update block", numberThreads) { }

    void execute(std::shared_ptr<GridBlock<Type>> block) override {
        auto result = std::make_shared<GridBlock<Type>>(block->blockSize(),
                block->gridWidth(), block->gridHeight(), block->blockRowIdx(),
                block->blockColIdx(), result_->grid());
        size_t iBegin = block->gridRowIdx();
        size_t iEnd = std::min(block->gridRowIdx() + block->blockSize(), block->gridHeight());
        size_t jBegin = block->gridColIdx();
        size_t jEnd = std::min(block->gridColIdx() + block->blockSize(), block->gridWidth());

        for (size_t i = iBegin; i < iEnd; ++i) {
            for (size_t j = jBegin; j < jEnd; ++j) {
                result->grid()[i * result->gridWidth() + j] = updateCell(i, j, block);
            }
        }
        this->addResult(result);
    }

    std::shared_ptr<hh::AbstractTask<1, GridBlock<Type>, GridBlock<Type>>>
    copy() override {
        return std::make_shared<UpdateBlockTask>(this->numberThreads(), result_);
    }

  private:
    Type updateCell(size_t row, size_t col, std::shared_ptr<GridBlock<Type>> block) {
        int countCell = 0;
        Type result = false;
        size_t iBegin = (row > 0)? row - 1 : 0;
        size_t iEnd = std::min(row + 1, block->gridHeight() - 1);
        size_t jBegin = (col > 0)? col - 1 : 0;
        size_t jEnd = std::min(col + 1, block->gridWidth() - 1);

        for (size_t i = iBegin; i <= iEnd; ++i) {
            for (size_t j = jBegin; j <= jEnd; ++j) {
                if (!(i == row && j == col)) {
                    if (block->grid()[i * block->gridWidth() + j]) {
                        ++countCell;
                    }
                }
            }
        }
        if (block->grid()[row * block->gridWidth() + col]) {
            result = (countCell == 2 || countCell == 3);
        } else {
            result = countCell == 3;
        }
        return result;
    }

    std::shared_ptr<Grid<Type>> result_;
};

#endif
