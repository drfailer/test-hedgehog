#ifndef MATRIX_BLOCK_DATA_HPP
#define MATRIX_BLOCK_DATA_HPP

#include <cstddef>

template <typename Type>
class MatrixBlockData {
  public:
    MatrixBlockData(size_t blockSize, size_t blockRow, size_t blockCol, Type *matrixData):
        blockSize(blockSize), blockRow(blockRow), blockCol(blockCol), matrixData(matrixData) {}
    ~MatrixBlockData() = default;

    size_t getBlockSize() const { return blockSize; }
    size_t getBlockRow() const { return blockRow; }
    size_t getBlockCol() const { return blockCol; }
    Type  *getMatrixData() const { return matrixData; }

  private:
    size_t blockSize = 0;
    size_t blockRow = 0;
    size_t blockCol = 0;
    Type *matrixData = nullptr;
};

#endif
