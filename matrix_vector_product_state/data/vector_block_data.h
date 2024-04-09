#ifndef VECTOR_BLOCK_DATA_HPP
#define VECTOR_BLOCK_DATA_HPP

#include "ids.h"
#include "vector_data.h"
#include <cstddef>
#include <memory>

template <typename Type, Ids Id>
class VectorBlockData {
  public:
    VectorBlockData(size_t blockSize, size_t blockRow, size_t blockCol, std::shared_ptr<VectorData<Type, Id>> const& vectorData):
        blockSize_(blockSize), blockRow_(blockRow), blockCol_(blockCol),
        vectorSize_(vectorData->getSize()),
        vectorData_(vectorData->getData()) {}
    ~VectorBlockData() = default;

    size_t getBlockSize() const { return blockSize_; }
    size_t getBlockRow() const { return blockRow_; }
    size_t getBlockCol() const { return blockCol_; }
    size_t getVectorSize() const { return vectorSize_; }
    Type  *getVectorData() const { return vectorData_; }

    friend std::ostream& operator<<(std::ostream& os, const VectorBlockData& vectorBlockData) {
        size_t begin = vectorBlockData.blockRow_;
        size_t end = std::min(begin + vectorBlockData.blockSize_, vectorBlockData.vectorSize_);
        for (size_t i = begin; i < end; ++i) {
            os << vectorBlockData.vectorData_[i + vectorBlockData.blockRow_] << std::endl;
        }
        return os;
    }

  private:
    size_t blockSize_ = 0;
    size_t blockRow_ = 0;
    size_t blockCol_ = 0;
    size_t vectorSize_ = 0;
    Type *vectorData_ = nullptr;
};

#endif
