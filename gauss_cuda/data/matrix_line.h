#ifndef MATRIX_LINE_HPP
#define MATRIX_LINE_HPP

#include "ids.h"
#include <cstddef>
#include <memory>

template <typename Type, Ids Id>
class MatrixLine { // => EquationLine
  public:
    MatrixLine(size_t size):
        size_(size) {}

    MatrixLine(size_t size, size_t row, Type *vectorValue, Type *ptr):
        size_(size), row_(row), vectorValue_(vectorValue), ptr_(ptr) {}

    template<Ids OtherId>
    MatrixLine(MatrixLine<Type, OtherId> const & ptr):
        MatrixLine(ptr->size(), ptr->row(), ptr->vectorValue(), ptr->get()) { }

    template<Ids OtherId>
    MatrixLine(std::shared_ptr<MatrixLine<Type, OtherId>> ptr):
        MatrixLine(ptr->size(), ptr->row(), ptr->vectorValue(), ptr->get()) { }

    size_t size() const { return size_; }
    size_t row() const { return row_; }
    Type *vectorValue() const { return vectorValue_; }
    Type *get() const { return ptr_; }
    Type **ptr() { return &ptr_; }
    Type **vectorValuePtr() { return &vectorValue_; }

    void size(size_t size) { this->size_ = size; }
    void row(size_t row) { this->row_ = row; }
    void vectorValue(Type *vectorValue) { this->vectorValue_ = vectorValue; }
    void set(Type *ptr) { this->ptr_ = ptr; }
    void vectorValue(Type newValue) { *this->vectorValue_ = newValue; }

  protected:
    size_t size_ = 0;
    size_t row_ = 0;
    Type *vectorValue_ = nullptr;
    Type *ptr_ = nullptr;
};

#endif
