#ifndef CUDA_SUBSTRACT_LINES_TASK_HPP
#define CUDA_SUBSTRACT_LINES_TASK_HPP

#include <hedgehog/hedgehog.h>
#include "../data/cuda_matrix_line.h"

template <typename Type>
using CudaSubLineInType = std::pair<std::shared_ptr<CudaMatrixLine<Type, Line>>, std::shared_ptr<CudaMatrixLine<Type, Line>>>;

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

    void execute(std::shared_ptr<CudaSubLineInType<Type>> lines) {
        std::shared_ptr<CudaMatrixLine<Type, Line>> pivotLine = lines->first;
        std::shared_ptr<CudaMatrixLine<Type, Line>> line = lines->second;
        auto result = std::dynamic_pointer_cast<CudaMatrixLine<Type, SubstractedLine>>(this->getManagedMemory());
        Type coef = line->get()[pivotLine->row()];

        result->ttl(1);

        if (coef) {
            for (size_t i = 0; i < pivotLine->size(); ++i) {
                result->get()[i] -= pivotLine->get()[i] * coef;
            }
            *result->vectorValue() -= *pivotLine->vectorValue() * coef;
        }
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
