#ifndef SUBSTRACT_PAIR_HPP
#define SUBSTRACT_PAIR_HPP

#include "ids.h"
#include "cuda_matrix_line.h"

template <typename Type>
class SubstractPair {
  public:
    SubstractPair(std::shared_ptr<CudaMatrixLine<Type, Line>> pivot,
            std::shared_ptr<CudaMatrixLine<Type, Line>> line, Type coef):
        pivot_(pivot), line_(line), coef_(coef) {}

    std::shared_ptr<CudaMatrixLine<Type, Line>> pivot() const { return pivot_; }
    std::shared_ptr<CudaMatrixLine<Type, Line>> line() const { return line_; }
    Type coef() const { return coef_; }

  private:
    std::shared_ptr<CudaMatrixLine<Type, Line>> pivot_;
    std::shared_ptr<CudaMatrixLine<Type, Line>> line_;
    Type coef_;
};

#endif
