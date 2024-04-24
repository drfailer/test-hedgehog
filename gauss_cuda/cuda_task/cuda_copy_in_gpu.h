#ifndef CUDA_COPY_IN_GPU_HPP
#define CUDA_COPY_IN_GPU_HPP

#include "../data/ids.h"
#include "../data/matrix_line.h"
#include "../data/cuda_matrix_line.h"
#include "cuda_substract_lines_task.h"
#include <cstddef>
#include <cuda.h>
#include <hedgehog/hedgehog.h>
#include <memory>

template <typename Type>
using SubLineInType = std::pair<std::shared_ptr<MatrixLine<Type, PivotLine>>, std::shared_ptr<MatrixLine<Type, Line>>>;

#define CudaCopyInGPUNbIn 1
#define CudaCopyInGPUInput SubLineInType<Type>
#define CudaCopyInGPUOutput CudaSubLineInType<Type>

template <typename Type>
class CudaCopyInGPU:
    public hh::AbstractCUDATask<CudaCopyInGPUNbIn, CudaCopyInGPUInput, CudaCopyInGPUOutput>
{
  public:
    CudaCopyInGPU(size_t lineTTL, size_t lineSize):
        hh::AbstractCUDATask<CudaCopyInGPUNbIn, CudaCopyInGPUInput, CudaCopyInGPUOutput>("Copy In GPU", 1, false, false),
        lineTTL_(lineTTL),
        lineSize_(lineSize)
    {}

    void execute(std::shared_ptr<SubLineInType<Type>> lines) override {
        auto pivotLine = std::dynamic_pointer_cast<CudaMatrixLine<Type, Line>>(this->getManagedMemory());
        auto line = std::dynamic_pointer_cast<CudaMatrixLine<Type, Line>>(this->getManagedMemory());

        pivotLine->fromMatrixLine(lines->first);
        line->fromMatrixLine(lines->second);
        pivotLine->ttl(1);
        line->ttl(1);

        Type coef = lines->second->get()[lines->first->row()];

        checkCudaErrors(cudaMemcpyAsync(pivotLine->get(), lines->first->get(),
                    sizeof(Type) * pivotLine->size(), cudaMemcpyHostToDevice, this->stream()));
        checkCudaErrors(cudaMemcpyAsync(pivotLine->vectorValue(), lines->first->vectorValue(),
                    sizeof(Type) * 1, cudaMemcpyHostToDevice, this->stream()));
        checkCudaErrors(cudaMemcpyAsync(line->get(), lines->second->get(),
                    sizeof(Type) * line->size(), cudaMemcpyHostToDevice, this->stream()));
        checkCudaErrors(cudaMemcpyAsync(line->vectorValue(), lines->second->vectorValue(),
                    sizeof(Type) * 1, cudaMemcpyHostToDevice, this->stream()));
        checkCudaErrors(cudaStreamSynchronize(this->stream()));
        this->addResult(std::make_shared<CudaSubLineInType<Type>>(pivotLine, line, coef));
    }

    std::shared_ptr<hh::AbstractTask<CudaCopyInGPUNbIn, CudaCopyInGPUInput, CudaCopyInGPUOutput>>
    copy() override {
        return std::make_shared<CudaCopyInGPU<Type>>(this->lineTTL_, this->lineSize_);
    }

  private: // TODO: may be useless
    size_t lineTTL_ = 0;
    size_t lineSize_ = 0;
};

#endif
