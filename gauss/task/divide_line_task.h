#ifndef DIVIDE_LINE_TASK_HPP
#define DIVIDE_LINE_TASK_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/matrix_line.h"

#define DivideLineTaskInNb 1
#define DivideLineTaskInput MatrixLine<Type, PivotLine>
#define DivideLineTaskOutput MatrixLine<Type, PivotLine>

template <typename Type>
class DivideLineTask: public hh::AbstractTask<DivideLineTaskInNb, DivideLineTaskInput, DivideLineTaskOutput> {
  public:
    DivideLineTask(size_t ): // NOTE: this task should not be parallelized if we are working with lines and not block of lines.
        hh::AbstractTask<DivideLineTaskInNb, DivideLineTaskInput, DivideLineTaskOutput>("Divide line task") {}

    void execute(std::shared_ptr<MatrixLine<Type, PivotLine>> line) override {
        Type coef = line->get()[line->row()];

        for (size_t i = line->row(); i < line->size(); ++i) {
            line->get()[i] /= coef;
        }
        *line->vectorValue() /= coef;
        this->addResult(std::make_shared<MatrixLine<Type, PivotLine>>(line));
    }

    std::shared_ptr<hh::AbstractTask<DivideLineTaskInNb, DivideLineTaskInput, DivideLineTaskOutput>>
    copy() override {
        return std::make_shared<DivideLineTask<Type>>(this->numberThreads());
    }
};

#endif
