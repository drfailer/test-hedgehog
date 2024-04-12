#ifndef PIVOT_STATE_MANAGER_HPP
#define PIVOT_STATE_MANAGER_HPP

#include <hedgehog/hedgehog.h>

#include "../data/ids.h"
#include "../data/matrix_line.h"
#include "pivot_state.h"

template <typename Type>
class PivotStateManager: public hh::StateManager<
                                    3,
                                    MatrixLine<Type, Line>, MatrixLine<Type, PivotLine>, MatrixLine<Type, SubstractedLine>,
                                    MatrixLine<Type, PivotLine>, MatrixLine<Type, Line>, MatrixLine<Type, ResultLine>> {
  public:
    PivotStateManager(std::shared_ptr<PivotState<Type>> const& state):
        hh::StateManager<
            3,
            MatrixLine<Type, Line>, MatrixLine<Type, PivotLine>, MatrixLine<Type, SubstractedLine>,
            MatrixLine<Type, PivotLine>, MatrixLine<Type, Line>, MatrixLine<Type, ResultLine>>(state, "Pivot State Manager") { }

    [[nodiscard]] bool canTerminate() const override {
        this->state()->lock();
        auto ret = std::dynamic_pointer_cast<PivotState<Type>>(this->state())->isDone();
        this->state()->unlock();
        return ret;
    }
};

#endif
