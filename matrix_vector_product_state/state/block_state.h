#ifndef BLOCK_STATE_HPP
#define BLOCK_STATE_HPP

#include <hedgehog/hedgehog.h>
#include <vector>
#include "../data/matrix_block_data.h"
#include "../data/vector_block_data.h"
#include "../data/product_block_data.h"
#include "../data/ids.h"

template <typename Type>
class BlockState: public hh::AbstractState<3,
    MatrixBlockData<Type, Matrix>, VectorBlockData<Type, Vector>, VectorBlockData<Type, Result>,
    ProductBlockData<Type>> {
  public:
      BlockState(size_t width, size_t height, size_t blockSize):
          width_(width), height_(height), blockSize_(blockSize),
          matrix_(width * height, nullptr),
          vector_(width, nullptr),
          result_(height, nullptr)
        {}

      void execute(std::shared_ptr<MatrixBlockData<Type, Matrix>> matrixBlock) {
          std::shared_ptr<VectorBlockData<Type, Vector>> vectorBlock = nullptr;
          std::shared_ptr<VectorBlockData<Type, Result>> resultBlock = nullptr;
          size_t row = matrixBlock->getBlockRow();
          size_t col = matrixBlock->getBlockCol();

          if ((vectorBlock = vec(row)) && (resultBlock = res(col))) {
              resetvec(row);
              resetres(col);
              auto productBlock = std::make_shared<ProductBlockData<Type>>();
              productBlock->matrixBlockData = matrixBlock;
              productBlock->vectorBlockData = vectorBlock;
              productBlock->resultBlockData = resultBlock;
              this->addResult(productBlock);
          } else {
              mat(matrixBlock);
          }
      }

      void execute(std::shared_ptr<VectorBlockData<Type, Vector>> vectorBlock) {
          std::shared_ptr<MatrixBlockData<Type, Matrix>> matrixBlock = nullptr;
          std::shared_ptr<VectorBlockData<Type, Result>> resultBlock = nullptr;
          size_t row = vectorBlock->getBlockRow();
          size_t col = vectorBlock->getBlockCol();

          if ((matrixBlock = mat(row, col)) && (resultBlock = res(col))) {
              resetmat(row, col);
              resetres(col);
              auto productBlock = std::make_shared<ProductBlockData<Type>>();
              productBlock->matrixBlockData = matrixBlock;
              productBlock->vectorBlockData = vectorBlock;
              productBlock->resultBlockData = resultBlock;
              this->addResult(productBlock);
          } else {
              vec(vectorBlock);
          }
      }

      void execute(std::shared_ptr<VectorBlockData<Type, Result>> resultBlock) {
          std::shared_ptr<MatrixBlockData<Type, Matrix>> matrixBlock = nullptr;
          std::shared_ptr<VectorBlockData<Type, Vector>> vectorBlock = nullptr;
          size_t row = resultBlock->getBlockRow();
          size_t col = resultBlock->getBlockCol();

          if ((matrixBlock = mat(row, col)) && (vectorBlock = vec(row))) {
              resetmat(row, col);
              resetvec(row);
              auto productBlock = std::make_shared<ProductBlockData<Type>>();
              productBlock->matrixBlockData = matrixBlock;
              productBlock->vectorBlockData = vectorBlock;
              productBlock->resultBlockData = resultBlock;
              this->addResult(productBlock);
          } else {
              res(resultBlock);
          }
      }

  private:
    std::vector<std::shared_ptr<MatrixBlockData<Type, Matrix>>> matrix_ = {};
    std::vector<std::shared_ptr<VectorBlockData<Type, Vector>>> vector_ = {};
    std::vector<std::shared_ptr<VectorBlockData<Type, Result>>> result_ = {};

    std::shared_ptr<MatrixBlockData<Type, Matrix>> mat(size_t row, size_t col) {
        return matrix_[(row / blockSize_) * width_ + col / blockSize_];
    }

    std::shared_ptr<VectorBlockData<Type, Vector>> vec(size_t row) {
        return vector_[row / blockSize_];
    }

    std::shared_ptr<VectorBlockData<Type, Result>> res(size_t col) {
        return result_[col / blockSize_];
    }

    void mat(std::shared_ptr<MatrixBlockData<Type, Matrix>> ptr) {
        matrix_[(ptr->getBlockRow() / blockSize_) * width_ + ptr->getBlockCol() / blockSize_] = ptr;
    }

    void vec(std::shared_ptr<VectorBlockData<Type, Vector>> ptr) {
        vector_[ptr->getBlockRow() / blockSize_] = ptr;
    }

    void res(std::shared_ptr<VectorBlockData<Type, Result>> ptr) {
        result_[ptr->getBlockRow() / blockSize_] = ptr;
    }

    void resetmat(size_t row, size_t col) { matrix_[(row / blockSize_) * width_ + col / blockSize_] = nullptr; }
    void resetvec(size_t row) { vector_[row / blockSize_] = nullptr; }
    void resetres(size_t col) { result_[col / blockSize_] = nullptr; }

    size_t width_ = 0;
    size_t height_ = 0;
    size_t blockSize_ = 0;
};

#endif
