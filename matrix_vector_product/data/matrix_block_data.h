#ifndef MATRIX_BLOCK_DATA_HPP
#define MATRIX_BLOCK_DATA_HPP

#include "matrix_data.h"
#include <cstddef>

template <typename Type>
class MatrixBlockData {
  public:
    MatrixBlockData(size_t blockSize, size_t blockRow, size_t blockCol, MatrixData<Type> const& matrixData):
        blockSize_(blockSize), blockRow_(blockRow), blockCol_(blockCol),
        matrixWidth_(matrixData.getWidth()),
        matrixHeight_(matrixData.getHeight()),
        matrixData_(matrixData.getData()) {}
    ~MatrixBlockData() = default;

    size_t getBlockSize() const { return blockSize_; }
    size_t getBlockRow() const { return blockRow_; }
    size_t getBlockCol() const { return blockCol_; }
    size_t getMatrixWidth() const { return matrixWidth_; }
    size_t getMatrixHeight() const { return matrixHeight_; }
    Type  *getMatrixData() const { return matrixData_; }

  private:
    size_t blockSize_ = 0;
    size_t blockRow_ = 0;
    size_t blockCol_ = 0;
    size_t matrixWidth_ = 0;
    size_t matrixHeight_ = 0;
    Type *matrixData_ = nullptr;
};

#endif
