#ifndef MATRIX_LINE_BLOCK_HPP
#define MATRIX_LINE_BLOCK_HPP

#include <cstddef>
#include <memory>
#include "ids.h"

template <typename Type, Ids Id>
class MatrixLineBlock {
  public:
    MatrixLineBlock(size_t lineWidth, size_t blockSize, size_t blockIdx, size_t row, Type *fullLinePtr, Type *vectorPtr):
        lineWidth_(lineWidth),
        blockSize_(blockSize),
        blockIdx_(blockIdx),
        row_(row),
        blockPtr_(fullLinePtr + blockIdx * blockSize),
        fullLinePtr_(fullLinePtr),
        vectorPtr_(vectorPtr) {}

    template <Ids OtherId>
    MatrixLineBlock(std::shared_ptr<MatrixLineBlock<Type, OtherId>> ptr):
        MatrixLineBlock(ptr->lineWidth(), ptr->blockSize(), ptr->blockIdx(),
                ptr->row(), ptr->fullLine(), ptr->vectorPtr())
    {}

    size_t lineWidth() const { return lineWidth_; }
    size_t blockSize() const { return blockSize_; }
    size_t blockIdx() const { return blockIdx_; }
    size_t row() const { return row_; }
    Type  *get() const { return blockPtr_; }
    Type  *fullLine() const { return fullLinePtr_; }
    Type  *vectorPtr() const { return vectorPtr_; }

  private:
    size_t lineWidth_ = 0;
    size_t blockSize_ = 0;
    size_t blockIdx_ = 0;
    size_t row_ = 0;
    Type *blockPtr_ = nullptr;
    Type *fullLinePtr_ = nullptr;
    Type *vectorPtr_ = nullptr;
};

#endif
