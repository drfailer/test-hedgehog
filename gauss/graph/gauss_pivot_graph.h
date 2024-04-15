#ifndef GAUSS_PIVOT_GRAPH_HPP
#define GAUSS_PIVOT_GRAPH_HPP

#include <cstddef>
#include <memory>
#include <hedgehog/hedgehog.h>
#include "../data/matrix.h"
#include "../data/vector.h"
#include "../task/sub_line_task.h"
#include "../task/divide_line_task.h"
#include "../task/split_matrix_task.h"
#include "../state/pivot_state.h"
#include "../state/pivot_state_manager.h"
#include "../state/substract_lines_state.h"
#include "../state/substract_lines_state_manager.h"
#include "../state/solver_state.h"
#include "../state/solver_state_manager.h"

template<typename Type>
class GaussPivotGraph: public hh::Graph<1, std::pair<Matrix<Type>, Vector<Type>>, MatrixLine<Type, ResultLine>> {
  public:
    GaussPivotGraph(size_t nbLines, size_t nbThreads):
        hh::Graph<1, std::pair<Matrix<Type>, Vector<Type>>, MatrixLine<Type, ResultLine>>() {
            auto splitMatrixTask = std::make_shared<SplitMatrixTask<Type>>();
            auto subLineTask = std::make_shared<SubLineTask<Type>>(nbThreads);
            auto divideLineTask = std::make_shared<DivideLineTask<Type>>(nbThreads);
            auto pivotState = std::make_shared<PivotState<Type>>(nbLines);
            auto pivotStateManager = std::make_shared<PivotStateManager<Type>>(pivotState);
            auto substractLinesState = std::make_shared<SubstractLinesState<Type>>(nbLines);
            auto substractLinesStateManager = std::make_shared<SubstractLinesStateManager<Type>>(substractLinesState);

            this->inputs(splitMatrixTask);
            this->edges(splitMatrixTask, pivotStateManager);
            this->edges(pivotStateManager, divideLineTask);
            this->edges(divideLineTask, pivotStateManager);
            this->edges(divideLineTask, substractLinesStateManager);
            this->edges(pivotStateManager, substractLinesStateManager);
            this->edges(substractLinesStateManager, subLineTask);
            this->edges(subLineTask, pivotStateManager);
            /* this->outputs(pivotStateManager); */

            auto solverState = std::make_shared<SolverState<Type>>(nbLines);
            auto solverStateManager = std::make_shared<SolverStateManager<Type>>(solverState);
            auto solv_subLineTask = std::make_shared<SubLineTask<Type>>(nbThreads);
            auto solv_substractLinesState = std::make_shared<SubstractLinesState<Type>>(nbLines);
            auto solv_substractLinesStateManager = std::make_shared<SubstractLinesStateManager<Type>>(solv_substractLinesState);

            this->edges(pivotStateManager, solverStateManager);
            this->edges(solverStateManager, solv_substractLinesStateManager);
            this->edges(solv_substractLinesStateManager, solv_subLineTask);
            this->edges(solv_subLineTask, solverStateManager);
            this->outputs(solverStateManager);
        }
};

#endif
