#ifndef PRODUCT_BLOCK_DATA_HPP
#define PRODUCT_BLOCK_DATA_HPP

#include "ids.h"
#include "matrix_block_data.h"
#include "vector_block_data.h"
#include <memory>

template <typename Type>
struct ProductBlockData {
    std::shared_ptr<MatrixBlockData<Type, Matrix>> matrixBlockData;
    std::shared_ptr<VectorBlockData<Type, Vector>> vectorBlockData;
    std::shared_ptr<VectorBlockData<Type, Result>> resultBlockData;
};

#endif
