#ifndef SOLVER_STATE_MANAGER_HPP
#define SOLVER_STATE_MANAGER_HPP

#include <hedgehog/hedgehog.h>
#include <memory>
#include "solver_state.h"

template<typename Type>
class SolverStateManager: public hh::StateManager<SolverStateInNb, SolverStateInput, SolverStateOutput> {
  public:
      SolverStateManager(std::shared_ptr<SolverState<Type>> const& state):
          hh::StateManager<SolverStateInNb, SolverStateInput, SolverStateOutput>(state, "Solver state manager") {}

    [[nodiscard]] bool canTerminate() const override {
        this->state()->lock();
        auto ret = std::dynamic_pointer_cast<SolverState<Type>>(this->state())->isDone();
        this->state()->unlock();
        return ret;
    }
};

#endif
