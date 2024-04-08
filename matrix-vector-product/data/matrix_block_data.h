#ifndef MATRIX_BLOCK_DATA_HPP
#define MATRIX_BLOCK_DATA_HPP

#include "matrix_data.h"
#include <cstddef>

template <typename Type>
class MatrixBlockData {
  public:
    MatrixBlockData(size_t blockSize, size_t blockRow, size_t blockCol, MatrixData<Type> const& matrixData):
        blockSize(blockSize), blockRow(blockRow), blockCol(blockCol),
        matrixWidth(matrixData.getWidth()),
        matrixHeight(matrixData.getHeight()),
        matrixData(matrixData.getData()) {}
    ~MatrixBlockData() = default;

    size_t getBlockSize() const { return blockSize; }
    size_t getBlockRow() const { return blockRow; }
    size_t getBlockCol() const { return blockCol; }
    size_t getMatrixWidth() const { return matrixWidth; }
    size_t getMatrixHeight() const { return matrixHeight; }
    Type  *getMatrixData() const { return matrixData; }

  private:
    size_t blockSize = 0;
    size_t blockRow = 0;
    size_t blockCol = 0;
    size_t matrixWidth = 0;
    size_t matrixHeight = 0;
    Type *matrixData = nullptr;
};

#endif
