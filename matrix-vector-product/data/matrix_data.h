#ifndef MATRIX_DATA_HPP
#define MATRIX_DATA_HPP

#include <cmath>
#include <cstddef>
#include <ostream>

template <typename Type>
class MatrixData {
  public:
    MatrixData(size_t width, size_t height, size_t blockSize, Type *data):
        width(width), height(height), blockSize(blockSize),
        numBlocksRows(std::ceil(height / blockSize) + (height % blockSize == 0 ? 0: 1)),
        numBlocksCols(std::ceil(width / blockSize) + (width % blockSize == 0 ? 0: 1)),
        data(data) {}
    ~MatrixData() = default;

    size_t getHeight() const { return height; }
    size_t getWidth() const { return width; }
    size_t getBlockSize() const { return blockSize; }
    size_t getNumBlocksRows() const { return numBlocksRows; }
    size_t getNumBlocksCols() const { return numBlocksCols; }
    Type  *getData() const { return data; }


    friend std::ostream& operator<<(std::ostream& os, const MatrixData& matrix) {
        for (size_t i = 0; i < matrix.height; ++i) {
            for (size_t j = 0; j < matrix.width; ++j) {
                os << matrix.getData()[i * matrix.width + j] << " ";
            }
            os << std::endl;
        }
        return os;
    }

  private:
    size_t width = 0;
    size_t height = 0;
    size_t blockSize = 0;
    size_t numBlocksRows = 0;
    size_t numBlocksCols = 0;
    Type *data = nullptr;
};

#endif
