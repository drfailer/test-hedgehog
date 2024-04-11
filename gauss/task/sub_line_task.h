#ifndef SUB_LINE_TASK_HPP
#define SUB_LINE_TASK_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/matrix_line.h"

template <typename Type>
using SubLineInType = std::pair<std::shared_ptr<MatrixLine<Type, PivotLine>>, std::shared_ptr<MatrixLine<Type, Line>>>;

template <typename Type>
class SubLineTask: public hh::AbstractTask<1, SubLineInType<Type>, MatrixLine<Type, SubstractedLine>> {
  public:
    SubLineTask(size_t nbThreads):
        hh::AbstractTask<1, SubLineInType<Type>, MatrixLine<Type, SubstractedLine>>("Substract lines", nbThreads) {}

    void execute(std::shared_ptr<SubLineInType<Type>> lines) override { // L2 = L2 - L1
        std::shared_ptr<MatrixLine<Type, PivotLine>> line1 = lines->first;
        std::shared_ptr<MatrixLine<Type, Line>> line2 = lines->second;
        auto result = std::make_shared<MatrixLine<Type, SubstractedLine>>(line2);
        Type coef = line2->get()[line1->row()];

        if (coef) {
            for (size_t i = 0; i < line1->size(); ++i) {
                result->get()[i] -= line1->get()[i] * coef;
            }
            *result->vectorValue() -= *line1->vectorValue() * coef;
        }
        this->addResult(result);
    }

    std::shared_ptr<hh::AbstractTask<1, SubLineInType<Type>, MatrixLine<Type, SubstractedLine>>>
    copy() override {
        return std::make_shared<SubLineTask<Type>>(this->numberThreads());
    }
};

#endif
