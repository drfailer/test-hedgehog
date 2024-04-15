#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <ostream>
#include <iostream>
#include <cstddef>

template <typename Type>
class Matrix {
  public:
    Matrix(size_t width, size_t height, Type *ptr):
        width_(width), height_(height), ptr_(ptr) {}
    ~Matrix() = default;

    size_t width() const { return width_; }
    size_t height() const { return height_; }
    Type *get() const { return ptr_; }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (size_t i = 0; i < matrix.height_; ++i) {
            for (size_t j = 0; j < matrix.width_; ++j) {
                os << matrix.ptr_[i * matrix.width_ + j] << " ";
            }
            os << std::endl;
        }
        return os;
    }

  private:
    size_t width_ = 0;
    size_t height_ = 0;
    Type *ptr_ = nullptr;
};

#endif
