#ifndef SPLIT_MATRIX_TASK_HPP
#define SPLIT_MATRIX_TASK_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include <utility>
#include <memory>
#include "../data/matrix.h"
#include "../data/matrix_line.h"
#include "../data/vector.h"
#include "../data/ids.h"

template <typename Type>
class SplitMatrixTask: public hh::AbstractTask<1, std::pair<Matrix<Type>, Vector<Type>>, MatrixLine<Type, Line>> {
  public:
    SplitMatrixTask():
        hh::AbstractTask<1, std::pair<Matrix<Type>, Vector<Type>>, MatrixLine<Type, Line>>("Split matrix.") {}

    void execute(std::shared_ptr<std::pair<Matrix<Type>, Vector<Type>>> input) override {
        auto matrix = input->first;
        auto vector = input->second;
        for (size_t i = 0; i < matrix.height(); ++i) {
            this->addResult(
                        std::make_shared<MatrixLine<Type, Line>>(
                            matrix.width(),
                            i,
                            &vector.get()[i],
                            matrix.get() + i * matrix.width())
                        );
        }
    }
};

#endif
