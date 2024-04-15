#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <ostream>
#include <cstddef>

template <typename Type>
class Vector {
  public:
    Vector(size_t size, Type *ptr):
        size_(size), ptr_(ptr) {}
    ~Vector() = default;

    size_t size() const { return size_; }
    Type *get() const { return ptr_; }

    friend std::ostream& operator<<(std::ostream& os, const Vector& vector) {
        for (size_t i = 0; i < vector.size_; ++i) {
            os << vector.ptr_[i] << std::endl;
        }
        return os;
    }

  private:
    size_t size_;
    Type *ptr_;
};

#endif
