#ifndef VECTOR_DATA_HPP
#define VECTOR_DATA_HPP

#include "ids.h"
#include <cmath>
#include <cstddef>
#include <ostream>

template <typename Type, Ids Id>
class VectorData {
  public:
    VectorData(size_t width, size_t height, size_t blockSize, Type *data):
        size_(Id == Vector ? width : height), blockSize_(blockSize),
        numBlockRows_(std::ceil(width / blockSize) + (width % blockSize == 0 ? 0 : 1)),
        numBlockCols_(std::ceil(height / blockSize) + (height % blockSize == 0 ? 0 : 1)),
        data_(data) {}
    ~VectorData() = default;

    size_t getSize() const { return size_; }
    size_t getBlockSize() const { return blockSize_; }
    size_t getNumBlockRows() const { return numBlockRows_; }
    size_t getNumBlockCols() const { return numBlockCols_; }
    Type  *getData() const { return data_; }

    friend std::ostream& operator<<(std::ostream& os, const VectorData& vector) {
        for (size_t i = 0; i < vector.size_; ++i) {
            os << vector.getData()[i] << std::endl;
        }
        return os;
    }

  private:
    size_t size_ = 0;
    size_t blockSize_ = 0;
    size_t numBlockRows_ = 0;
    size_t numBlockCols_ = 0;
    Type *data_ = nullptr;

};

#endif
