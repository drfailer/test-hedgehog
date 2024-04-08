#ifndef VECTOR_BLOCK_DATA_HPP
#define VECTOR_BLOCK_DATA_HPP

#include <cstddef>

template <typename Type>
class VectorBlockData {
  public:
    VectorBlockData(size_t blockSize, size_t blockRow, size_t blockCol, Type *vectorData):
        blockSize(blockSize), blockRow(blockRow), blockCol(blockCol), vectorData(vectorData) {}
    ~VectorBlockData() = default;

    size_t getBlockSize() const { return blockSize; }
    size_t getBlockRow() const { return blockRow; }
    size_t getBlockCol() const { return blockCol; }
    Type  *getVectorData() const { return vectorData; }

  private:
    size_t blockSize = 0;
    size_t blockRow = 0;
    size_t blockCol = 0;
    Type *vectorData = nullptr;
};

#endif
