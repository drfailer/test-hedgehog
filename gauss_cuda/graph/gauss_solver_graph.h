#ifndef GAUSS_SOLVER_GRAPH_HPP
#define GAUSS_SOLVER_GRAPH_HPP

#include <cstddef>
#include <memory>
#include <hedgehog/hedgehog.h>
#include "../cuda_task/cuda_substract_lines_task.h"
#include "../cuda_task/cuda_copy_in_gpu.h"
#include "../cuda_task/cuda_copy_out_gpu.h"
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
    GaussSolverGraph(size_t size, size_t nbThreads):
        hh::Graph<GaussSolverGraphInNb, GaussSolverGraphInput, GaussSolverGraphOutput>("Solver graph")
    {
        // cuda tasks
        auto copyLineInGPU = std::make_shared<CudaCopyInGPU<Type>>(size, size, nbThreads);
        auto copyLineOutGPU = std::make_shared<CudaCopyOutGPU<Type, SubstractedLine>>(nbThreads);
        auto subLineTask = std::make_shared<CudaSubstractLinesTask<Type>>(nbThreads);
        // states
        auto solverState = std::make_shared<SolverState<Type>>(size);
        auto solverStateManager = std::make_shared<SolverStateManager<Type>>(solverState);
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

        this->inputs(solverStateManager);
        this->edges(solverStateManager, substractLinesStateManager);

        // gpu computation
        this->edges(substractLinesStateManager, copyLineInGPU);
        this->edges(copyLineInGPU, subLineTask);
        this->edges(subLineTask, copyLineOutGPU);

        this->edges(copyLineOutGPU, solverStateManager);
        this->outputs(solverStateManager);
    }
};

#endif
