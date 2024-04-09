#ifndef GRID_BLOCK_HPP
#define GRID_BLOCK_HPP

#include <cstddef>
#include <cstring>

template <typename Type>
class GridBlock {
  public:
    GridBlock(size_t blockSize, size_t gridWidth, size_t gridHeight, size_t blockRowIdx,
            size_t blockColIdx, Type *fullGrid):
        blockSize_(blockSize),
        gridRowIdx_(blockRowIdx * blockSize),
        gridColIdx_(blockColIdx * blockSize),
        blockRowIdx_(blockRowIdx),
        blockColIdx_(blockColIdx),
        gridWidth_(gridWidth),
        gridHeight_(gridHeight),
        fullGrid_(fullGrid) {
        }

    size_t blockSize() const { return blockSize_; }
    size_t gridRowIdx() const { return gridRowIdx_; }
    size_t gridColIdx() const { return gridColIdx_; }
    size_t blockRowIdx() const { return blockRowIdx_; }
    size_t blockColIdx() const { return blockColIdx_; }
    size_t gridWidth() const { return gridWidth_; }
    size_t gridHeight() const { return gridHeight_; }
    Type  *grid() const { return fullGrid_; }

  private:
    size_t blockSize_ = 0;
    size_t gridRowIdx_ = 0;
    size_t gridColIdx_ = 0;
    size_t blockRowIdx_ = 0;
    size_t blockColIdx_ = 0;
    size_t gridWidth_ = 0;
    size_t gridHeight_ = 0;
    Type *fullGrid_ = nullptr;
};

#endif
