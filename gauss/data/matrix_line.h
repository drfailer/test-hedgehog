#ifndef MATRIX_LINE_HPP
#define MATRIX_LINE_HPP

#include "ids.h"
#include <cstddef>
#include <memory>

template <typename Type, Ids Id>
class MatrixLine { // => EquationLine
  public:
    MatrixLine(size_t size, size_t row, Type *vectorValue, Type *ptr):
        size_(size), row_(row), vectorValue_(vectorValue), ptr_(ptr) {}

    template<Ids OtherId>
    MatrixLine(std::shared_ptr<MatrixLine<Type, OtherId>> ptr):
        MatrixLine(ptr->size(), ptr->row(), ptr->vectorValue(), ptr->get()) {

    }

    size_t size() const { return size_; }
    size_t row() const { return row_; }
    Type *vectorValue() const { return vectorValue_; }
    Type *get() const { return ptr_; }

    void vectorValue(Type newValue) {
        *this->vectorValue_ = newValue;
    }

  private:
    size_t size_ = 0;
    size_t row_ = 0;
    Type *vectorValue_ = 0;
    Type *ptr_ = nullptr;
};

#endif
