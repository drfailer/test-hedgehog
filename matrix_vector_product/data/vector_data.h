#ifndef VECTOR_DATA_HPP
#define VECTOR_DATA_HPP

#include <cmath>
#include <cstddef>
#include <ostream>

template <typename Type>
class VectorData {
  public:
    VectorData(size_t size, size_t blockSize, Type *data):
        size_(size), blockSize_(blockSize),
        numBlocks_(std::ceil(size / blockSize) + (size % blockSize == 0 ? 0 : 1)),
        data_(data) {}
    ~VectorData() = default;

    size_t getSize() const { return size_; }
    size_t getBlockSize() const { return blockSize_; }
    size_t getNumBlocks() const { return numBlocks_; }
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
    size_t numBlocks_ = 0;
    Type *data_ = nullptr;

};

#endif
