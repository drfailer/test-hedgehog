#ifndef VECTOR_DATA_HPP
#define VECTOR_DATA_HPP

#include <cmath>
#include <cstddef>
#include <ostream>

template <typename Type>
class VectorData {
  public:
    VectorData(size_t size, size_t blockSize, Type *data):
        size(size), blockSize(blockSize),
        numBlocks(std::ceil(size / blockSize) + (size % blockSize == 0 ? 0 : 1)),
        data(data) {}
    ~VectorData() = default;

    size_t getSize() const { return size; }
    size_t getBlockSize() const { return blockSize; }
    size_t getNumBlocks() const { return numBlocks; }
    Type  *getData() const { return data; }

    friend std::ostream& operator<<(std::ostream& os, const VectorData& vector) {
        for (size_t i = 0; i < vector.size; ++i) {
            os << vector.getData()[i] << std::endl;
        }
        return os;
    }

  private:
    size_t size = 0;
    size_t blockSize = 0;
    size_t numBlocks = 0;
    Type *data = nullptr;

};

#endif
