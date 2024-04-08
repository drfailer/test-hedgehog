#ifndef PRODUCT_BLOCK_DATA_HPP
#define PRODUCT_BLOCK_DATA_HPP

#include "matrix_block_data.h"
#include "vector_block_data.h"
#include <memory>

template <typename Type>
struct ProductBlockData {
    std::shared_ptr<MatrixBlockData<Type>> matrixBlockData;
    std::shared_ptr<VectorBlockData<Type>> vectorBlockData;
    std::shared_ptr<VectorBlockData<Type>> resultBlockData;
};

#endif
