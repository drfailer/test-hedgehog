#ifndef MATRIXPRODUCT_TASK_HPP
#define MATRIXPRODUCT_TASK_HPP

#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/product_block_data.h"
#include "../data/vector_block_data.h"

template <typename Type>
class MatrixProductTask: public hh::AbstractTask<1, ProductBlockData<Type>, VectorBlockData<Type>> {
  public:
    MatrixProductTask(std::string const& name, size_t nbThreads):
        hh::AbstractTask<1, ProductBlockData<Type>, VectorBlockData<Type>>(name, nbThreads) {}
    ~MatrixProductTask() = default;

    void execute(std::shared_ptr<ProductBlockData<Type>> productData) override {
        auto matrixBlocData = productData->matrixBlockData;
        auto vectorBlockData = productData->vectorBlockData;
        auto resultBlocData = productData->resultBlockData;

        auto matrixData = matrixBlocData->getMatrixData();
        auto vectorData = vectorBlockData->getVectorData();
        auto resultData = resultBlocData->getVectorData();

        size_t rowIdx = matrixBlocData->getBlockRow();
        size_t colIdx = matrixBlocData->getBlockCol();
        size_t blockSize = matrixBlocData->getBlockSize();
        size_t matrixWidth = matrixBlocData->getMatrixWidth();
        size_t matrixHeight = matrixBlocData->getMatrixHeight();

        size_t ibegin = rowIdx, iend = std::min(rowIdx + blockSize, matrixHeight);
        size_t jbegin = colIdx, jend = std::min(colIdx + blockSize, matrixWidth);

        for (size_t i = ibegin; i < iend; ++i) {
            for (size_t j = jbegin; j < jend; ++j) {
                resultData[i] += matrixData[i * matrixWidth + j] * vectorData[j]; // need a lock ?
            }
        }
        this->addResult(resultBlocData);
    }

    std::shared_ptr<hh::AbstractTask<1, ProductBlockData<Type>, VectorBlockData<Type>>> copy() override {
        return std::make_shared<MatrixProductTask<Type>>(this->name(), this->numberThreads());
    }
};

#endif
