#ifndef PRODUCT_DATA_HPP
#define PRODUCT_DATA_HPP

#include "matrix_data.h"
#include "vector_data.h"

template <typename Type>
class ProductData {
  public:
    ProductData() = default;
    ~ProductData() = default;

    MatrixData<Type> const& getMatrix_data() const { return matrix_data; }
    VectorData<Type> const& getVector_data() const { return vector_data; }
    VectorData<Type> const& getResult_data() const { return result_data; }

  private:
    MatrixData<Type> matrix_data;
    VectorData<Type> vector_data;
    VectorData<Type> result_data;
};

#endif
