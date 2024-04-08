#ifndef VECTOR_BLOCK_DATA_HPP
#define VECTOR_BLOCK_DATA_HPP

#include "vector_data.h"
#include <cstddef>

template <typename Type>
class VectorBlockData {
  public:
    VectorBlockData(size_t blockSize, size_t blockRow, VectorData<Type> const& vectorData):
        blockSize(blockSize), blockRow(blockRow),
        vectorSize(vectorData.getSize()),
        vectorData(vectorData.getData()) {}
    ~VectorBlockData() = default;

    size_t getBlockSize() const { return blockSize; }
    size_t getBlockRow() const { return blockRow; }
    size_t getVectorSize() const { return vectorSize; }
    Type  *getVectorData() const { return vectorData; }

    friend std::ostream& operator<<(std::ostream& os, const VectorBlockData& vectorBlockData) {
        size_t begin = vectorBlockData.blockRow;
        size_t end = std::min(begin + vectorBlockData.blockSize, vectorBlockData.vectorSize);
        for (size_t i = begin; i < end; ++i) {
            os << vectorBlockData.vectorData[i + vectorBlockData.blockRow] << std::endl;
        }
        return os;
    }

  private:
    size_t blockSize = 0;
    size_t blockRow = 0;
    size_t vectorSize = 0;
    Type *vectorData = nullptr;
};

#endif
