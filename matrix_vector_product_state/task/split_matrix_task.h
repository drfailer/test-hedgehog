#ifndef SPLIT_MATRIX_TASK_HPP
#define SPLIT_MATRIX_TASK_HPP

#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/matrix_data.h"
#include "../data/matrix_block_data.h"

template <typename Type, Ids Id>
class SplitMatrixTask: public hh::AbstractTask<1, MatrixData<Type, Id>, MatrixBlockData<Type, Id>> {
  public:
    SplitMatrixTask(): hh::AbstractTask<1, MatrixData<Type, Id>, MatrixBlockData<Type, Id>>("Split") {}

    void execute(std::shared_ptr<MatrixData<Type, Id>> matrixData) override {
        size_t numblockRows = matrixData->getNumBlocksRows();
        size_t numblockCols = matrixData->getNumBlocksCols();
        size_t blockSize = matrixData->getBlockSize();

        for (size_t iGrid = 0; iGrid < numblockRows; ++iGrid) {
            for (size_t jGrid = 0; jGrid < numblockCols; ++jGrid) {
                auto matrixBlockData = std::make_shared<MatrixBlockData<Type, Id>>(
                        blockSize, iGrid * blockSize, jGrid * blockSize, matrixData);
                this->addResult(matrixBlockData);
            }
        }
    }
};

#endif
