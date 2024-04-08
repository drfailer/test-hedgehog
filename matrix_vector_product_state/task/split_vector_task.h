#ifndef SPLIT_VECTOR_TASK_HPP
#define SPLIT_VECTOR_TASK_HPP

#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/vector_data.h"
#include "../data/vector_block_data.h"

template <typename Type, Ids Id>
class SplitVectorTask: public hh::AbstractTask<1, VectorData<Type, Id>, VectorBlockData<Type, Id>> {
  public:
    SplitVectorTask(): hh::AbstractTask<1, VectorData<Type, Id>, VectorBlockData<Type, Id>>("Split") {}

    void execute(std::shared_ptr<VectorData<Type, Id>> vectorData) override {
        size_t numblockRows = vectorData->getNumBlockRows();
        size_t numblockCols = vectorData->getNumBlockCols();
        size_t blockSize = vectorData->getBlockSize();

        for (size_t iGrid = 0; iGrid < numblockRows; ++iGrid) {
            for (size_t jGrid = 0; jGrid < numblockCols; ++jGrid) {
                auto vectorBlockData = std::make_shared<VectorBlockData<Type, Id>>(
                        blockSize, iGrid * blockSize, jGrid * blockSize, vectorData);
                this->addResult(vectorBlockData);
            }
        }
    }
};

#endif

