#ifndef MATRIX_DATA_HPP
#define MATRIX_DATA_HPP

#include <cmath>
#include <cstddef>
#include <ostream>

template <typename Type>
class MatrixData {
  public:
    MatrixData(size_t width, size_t height, size_t blockSize, Type *data):
        width_(width), height_(height), blockSize_(blockSize),
        numBlocksRows_(std::ceil(height / blockSize) + (height % blockSize == 0 ? 0: 1)),
        numBlocksCols_(std::ceil(width / blockSize) + (width % blockSize == 0 ? 0: 1)),
        data_(data) {}
    ~MatrixData() = default;

    size_t getHeight() const { return height_; }
    size_t getWidth() const { return width_; }
    size_t getBlockSize() const { return blockSize_; }
    size_t getNumBlocksRows() const { return numBlocksRows_; }
    size_t getNumBlocksCols() const { return numBlocksCols_; }
    Type  *getData() const { return data_; }


    friend std::ostream& operator<<(std::ostream& os, const MatrixData& matrix) {
        for (size_t i = 0; i < matrix.height_; ++i) {
            for (size_t j = 0; j < matrix.width_; ++j) {
                os << matrix.getData()[i * matrix.width_ + j] << " ";
            }
            os << std::endl;
        }
        return os;
    }

  private:
    size_t width_ = 0;
    size_t height_ = 0;
    size_t blockSize_ = 0;
    size_t numBlocksRows_ = 0;
    size_t numBlocksCols_ = 0;
    Type *data_ = nullptr;
};

#endif
