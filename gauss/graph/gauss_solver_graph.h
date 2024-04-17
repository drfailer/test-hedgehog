#ifndef GAUSS_SOLVER_GRAPH_HPP
#define GAUSS_SOLVER_GRAPH_HPP

#include <cstddef>
#include <memory>
#include <hedgehog/hedgehog.h>
#include "../task/sub_line_task.h"
#include "../state/substract_lines_state.h"
#include "../state/substract_lines_state_manager.h"
#include "../state/solver_state.h"
#include "../state/solver_state_manager.h"

#define GaussSolverGraphInNb 1
#define GaussSolverGraphInput MatrixLine<Type, PivotedLine>
#define GaussSolverGraphOutput MatrixLine<Type, ResultLine>

template <typename Type>
class GaussSolverGraph: public hh::Graph<GaussSolverGraphInNb, GaussSolverGraphInput, GaussSolverGraphOutput> {
  public:
    GaussSolverGraph(size_t nbLines, size_t nbThreads):
        hh::Graph<GaussSolverGraphInNb, GaussSolverGraphInput, GaussSolverGraphOutput>("Solver graph")
    {
        auto solverState = std::make_shared<SolverState<Type>>(nbLines);
        auto solverStateManager = std::make_shared<SolverStateManager<Type>>(solverState);
        auto subLineTask = std::make_shared<SubLineTask<Type>>(nbThreads);
        auto substractLinesState = std::make_shared<SubstractLinesState<Type>>(nbLines);
        auto substractLinesStateManager = std::make_shared<SubstractLinesStateManager<Type>>(substractLinesState);

        this->inputs(solverStateManager);
        this->edges(solverStateManager, substractLinesStateManager);
        this->edges(substractLinesStateManager, subLineTask);
        this->edges(subLineTask, solverStateManager);
        this->outputs(solverStateManager);
    }
};

#endif
