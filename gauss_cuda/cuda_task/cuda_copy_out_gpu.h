#ifndef CUDA_COPY_OUT_GPU_HPP
#define CUDA_COPY_OUT_GPU_HPP

#include <hedgehog/hedgehog.h>
#include "../data/cuda_matrix_line.h"
#include "../data/matrix_line.h"
#include "../data/ids.h"

#define CudaCopyOutGPUNbIn 1
#define CudaCopyOutGPUInput CudaMatrixLine<Type, Id>
#define CudaCopyOutGPUOutput MatrixLine<Type, Id>

template <typename Type, Ids Id>
class CudaCopyOutGPU: public hh::AbstractCUDATask<CudaCopyOutGPUNbIn, CudaCopyOutGPUInput, CudaCopyOutGPUOutput> {
  public:
    CudaCopyOutGPU(size_t numberThreads):
        hh::AbstractCUDATask<CudaCopyOutGPUNbIn, CudaCopyOutGPUInput, CudaCopyOutGPUOutput>("Copy Out GPU", numberThreads, false, false)
    {}

    void execute(std::shared_ptr<CudaMatrixLine<Type, Id>> ptr) override {
        auto ret = ptr->copyToCPUMemory(this->stream());
        ptr->returnToMemoryManager();
        this->addResult(ret);
    }

    std::shared_ptr<hh::AbstractTask<CudaCopyOutGPUNbIn, CudaCopyOutGPUInput, CudaCopyOutGPUOutput>>
    copy() override {
        return std::make_shared<CudaCopyOutGPU<Type, Id>>(this->numberThreads());
    }
};

#endif
