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
      BlockState(size_t nbRows, size_t nbCols, size_t blockSize):
          nbRows_(nbRows), nbCols_(nbCols), blockSize_(blockSize),
          matrix_(nbRows * nbCols, nullptr),
          vector_(nbRows * nbCols, nullptr),
          result_(nbRows * nbCols, nullptr)
        {}

      void execute(std::shared_ptr<MatrixBlockData<Type, Matrix>> matrixBlock) {
          std::shared_ptr<VectorBlockData<Type, Vector>> vectorBlock = nullptr;
          std::shared_ptr<VectorBlockData<Type, Result>> resultBlock = nullptr;
          size_t row = matrixBlock->getBlockRow();
          size_t col = matrixBlock->getBlockCol();

          if ((vectorBlock = vec(row, col)) && (resultBlock = res(row, col))) {
              resetvec(row, col);
              resetres(row, col);
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

          if ((matrixBlock = mat(row, col)) && (resultBlock = res(row, col))) {
              resetmat(row, col);
              resetres(row, col);
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

          if ((matrixBlock = mat(row, col)) && (vectorBlock = vec(row, col))) {
              resetmat(row, col);
              resetvec(row, col);
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
        size_t rIdx = row / blockSize_;
        size_t cIdx = col / blockSize_;
        return matrix_[rIdx * nbRows_ + cIdx];
    }

    std::shared_ptr<VectorBlockData<Type, Vector>> vec(size_t row, size_t col) {
        size_t rIdx = row / blockSize_;
        size_t cIdx = col / blockSize_;
        return vector_[rIdx * nbRows_ + cIdx];
    }

    std::shared_ptr<VectorBlockData<Type, Result>> res(size_t row, size_t col) {
        size_t rIdx = row / blockSize_;
        size_t cIdx = col / blockSize_;
        return result_[rIdx * nbRows_ + cIdx];
    }

    void mat(std::shared_ptr<MatrixBlockData<Type, Matrix>> ptr) {
        size_t rIdx = ptr->getBlockRow() / blockSize_;
        size_t cIdx = ptr->getBlockCol() / blockSize_;
        matrix_[rIdx * nbRows_ + cIdx] = ptr;
    }

    void vec(std::shared_ptr<VectorBlockData<Type, Vector>> ptr) {
        size_t rIdx = ptr->getBlockRow() / blockSize_;
        size_t cIdx = ptr->getBlockCol() / blockSize_;
        vector_[rIdx * nbRows_ + cIdx] = ptr;
    }

    void res(std::shared_ptr<VectorBlockData<Type, Result>> ptr) {
        size_t rIdx = ptr->getBlockRow() / blockSize_;
        size_t cIdx = ptr->getBlockCol() / blockSize_;
        result_[rIdx * nbRows_ + cIdx] = ptr;
    }

    void resetmat(size_t row, size_t col) {
        size_t rIdx = row / blockSize_;
        size_t cIdx = col / blockSize_;
        matrix_[rIdx * nbRows_ + cIdx] = nullptr;
    }

    void resetvec(size_t row, size_t col) {
        size_t rIdx = row / blockSize_;
        size_t cIdx = col / blockSize_;
        vector_[rIdx * nbRows_ + cIdx] = nullptr;
    }

    void resetres(size_t row, size_t col) {
        size_t rIdx = row / blockSize_;
        size_t cIdx = col / blockSize_;
        result_[rIdx * nbRows_ + cIdx] = nullptr;
    }

    size_t nbRows_ = 0;
    size_t nbCols_ = 0;
    size_t blockSize_ = 0;
};

#endif
