#ifndef GAUSS_PIVOT_GRAPH_HPP
#define GAUSS_PIVOT_GRAPH_HPP

#include <cstddef>
#include <memory>
#include <hedgehog/hedgehog.h>
#include "../data/ids.h"
#include "../data/matrix.h"
#include "../data/vector.h"
#include "../task/divide_line_task.h"
#include "../task/split_matrix_task.h"
#include "../cuda_task/cuda_substract_lines_task.h"
#include "../cuda_task/cuda_copy_in_gpu.h"
#include "../cuda_task/cuda_copy_out_gpu.h"
#include "../state/pivot_state.h"
#include "../state/pivot_state_manager.h"
#include "../state/substract_lines_state.h"
#include "../state/substract_lines_state_manager.h"

#define GaussPivotGraphInNb 1
#define GaussPivotGraphInput std::pair<Matrix<Type>, Vector<Type>>
#define GaussPivotGraphOutput MatrixLine<Type, PivotedLine>

template<typename Type>
class GaussPivotGraph: public hh::Graph<GaussPivotGraphInNb, GaussPivotGraphInput, GaussPivotGraphOutput> {
  public:
    GaussPivotGraph(size_t size, size_t nbThreads):
        hh::Graph<GaussPivotGraphInNb, GaussPivotGraphInput, GaussPivotGraphOutput>("Pivot graph")
    {
        auto splitMatrixTask = std::make_shared<SplitMatrixTask<Type>>();
        auto divideLineTask = std::make_shared<DivideLineTask<Type>>(nbThreads);
        // cuda tasks
        auto copyLineInGPU = std::make_shared<CudaCopyInGPU<Type>>(size, size);
        auto copyLineOutGPU = std::make_shared<CudaCopyOutGPU<Type, SubstractedLine>>();
        auto subLineTask = std::make_shared<CudaSubstractLinesTask<Type>>(nbThreads);
        // states
        auto pivotState = std::make_shared<PivotState<Type>>(size);
        auto pivotStateManager = std::make_shared<PivotStateManager<Type>>(pivotState);
        auto substractLinesState = std::make_shared<SubstractLinesState<Type>>(size);
        auto substractLinesStateManager = std::make_shared<SubstractLinesStateManager<Type>>(substractLinesState);
        // memory managers
        auto mmLine =
            std::make_shared<hh::StaticMemoryManager<CudaMatrixLine<Type, Line>, size_t>>(size, size);
        auto mmSubstractedLine =
            std::make_shared<hh::StaticMemoryManager<CudaMatrixLine<Type, SubstractedLine>, size_t>>(size, size);

        // connect memory managers
        copyLineInGPU->connectMemoryManager(mmLine);
        subLineTask->connectMemoryManager(mmSubstractedLine);

        // graph

        this->inputs(splitMatrixTask);
        this->edges(splitMatrixTask, pivotStateManager);
        this->edges(pivotStateManager, divideLineTask);
        this->edges(pivotStateManager, substractLinesStateManager);
        this->edges(divideLineTask, pivotStateManager);
        this->edges(divideLineTask, substractLinesStateManager);

        // gpu computation
        this->edges(substractLinesStateManager, copyLineInGPU);
        this->edges(copyLineInGPU, subLineTask);
        this->edges(subLineTask, copyLineOutGPU);
        this->edges(copyLineOutGPU, pivotStateManager);

        this->outputs(pivotStateManager);
    }
};

#endif
