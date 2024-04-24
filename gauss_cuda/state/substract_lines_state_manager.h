#ifndef SUBSTRACT_LINES_STATE_MANAGER_HPP
#define SUBSTRACT_LINES_STATE_MANAGER_HPP

#include <hedgehog/hedgehog.h>
#include "substract_lines_state.h"

template <typename Type>
class SubstractLinesStateManager: public hh::StateManager<SubstractLinesStateInNb,
                                                          SubstractLinesStateInput,
                                                          SubstractLinesStateOutput> {
  public:
      SubstractLinesStateManager(std::shared_ptr<SubstractLinesState<Type>> const& state):
          hh::StateManager<SubstractLinesStateInNb,
                           SubstractLinesStateInput,
                           SubstractLinesStateOutput>(state, "Substract lines state manager") {}

    [[nodiscard]] bool canTerminate() const override {
        this->state()->lock();
        auto ret = std::dynamic_pointer_cast<SubstractLinesState<Type>>(this->state())->isDone();
        this->state()->unlock();
        return ret;
    }
};

#endif
