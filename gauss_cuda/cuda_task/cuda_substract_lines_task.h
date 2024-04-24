#ifndef CUDA_SUBSTRACT_LINES_TASK_HPP
#define CUDA_SUBSTRACT_LINES_TASK_HPP

#include <hedgehog/hedgehog.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include "../data/cuda_matrix_line.h"
#include "../data/substract_pair.h"
#include "../cuda/cuda_gauss.h"

template <typename Type>
using CudaSubLineInType = SubstractPair<Type>;

#define CudaSubLineTaskInNb 1
#define CudaSubLineTaskInput CudaSubLineInType<Type>
#define CudaSubLineTaskOutput CudaMatrixLine<Type, SubstractedLine>

template <typename Type>
class CudaSubstractLinesTask: public hh::AbstractCUDATask<CudaSubLineTaskInNb, CudaSubLineTaskInput, CudaSubLineTaskOutput> {
  public:
    CudaSubstractLinesTask(size_t nbThreads):
        hh::AbstractCUDATask<
            CudaSubLineTaskInNb,
            CudaSubLineTaskInput,
            CudaSubLineTaskOutput>("Cuda sub lines task", nbThreads, false, false) {}

    void execute(std::shared_ptr<SubstractPair<Type>> pair) override {
        std::shared_ptr<CudaMatrixLine<Type, Line>> pivotLine = pair->pivot();
        std::shared_ptr<CudaMatrixLine<Type, Line>> line = pair->line();
        auto result = std::dynamic_pointer_cast<CudaMatrixLine<Type, SubstractedLine>>(this->getManagedMemory());
        result->fromCudaMatrixLine(line);
        Type coef = pair->coef();
        int threadsPerBlock = 256;
        int blocksPerGrid = (pivotLine->size() + threadsPerBlock - 1) / threadsPerBlock;
        result->ttl(1);

        substractLines(result->get(), pivotLine->get(), result->vectorValue(),
                pivotLine->vectorValue(), coef, pivotLine->size(), this->stream());
        checkCudaErrors(cudaStreamSynchronize(this->stream()));
        pivotLine->returnToMemoryManager();
        line->returnToMemoryManager();
        this->addResult(result);
    }

    std::shared_ptr<hh::AbstractTask<CudaSubLineTaskInNb, CudaSubLineTaskInput, CudaSubLineTaskOutput>>
    copy() override {
        return std::make_shared<CudaSubstractLinesTask<Type>>(this->numberThreads());
    }
};

#endif
