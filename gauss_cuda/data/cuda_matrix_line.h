#ifndef CUDA_MATRIX_LINE_HPP
#define CUDA_MATRIX_LINE_HPP

#include "ids.h"
#include "matrix_line.h"
#include <hedgehog/hedgehog.h>
#include <cuda_runtime_api.h>

template <typename Type, Ids Id>
class CudaMatrixLine: public MatrixLine<Type, Id>,
                      public hh::ManagedMemory {
  public:
    CudaMatrixLine(size_t lineSize): MatrixLine<Type, Id>(lineSize) {
        checkCudaErrors(cudaMalloc((void **) &this->ptr_, sizeof(Type) * lineSize));
        checkCudaErrors(cudaMalloc((void **) &this->vectorValue_, sizeof(Type) * 1));
    }

    ~CudaMatrixLine() {
        checkCudaErrors(cudaFree(this->ptr_));
        checkCudaErrors(cudaFree(this->vectorValue_));
    }

    Type *hostVectorValuePtr() const { return hostVectorValuePtr_; };
    Type *hostLinePtr() const { return hostLinePtr_; };

    void recordEvent(cudaStream_t stream) { cudaEventRecord(event_, stream); }
    void synchronizeEvent() { cudaEventSynchronize(event_); }

    void ttl(size_t ttl) { ttl_ = ttl; }
    void postProcess() override { --this->ttl_; }
    bool canBeRecycled() override { return this->ttl_ == 0; }

    std::shared_ptr<MatrixLine<Type, Id>> copyToCPUMemory(cudaStream_t stream) {
        auto res = std::make_shared<MatrixLine<Type, Id>>(this->size(),
                this->row(), this->hostVectorValuePtr_, this->hostLinePtr_);
        checkCudaErrors(cudaMemcpyAsync(res->get(), this->get(),
                    res->size() * sizeof(Type), cudaMemcpyDeviceToHost, stream));
        checkCudaErrors(cudaMemcpyAsync(res->vectorValue(), this->vectorValue(),
                    sizeof(Type) * 1, cudaMemcpyDeviceToHost, stream));
        checkCudaErrors(cudaStreamSynchronize(stream));
        return res;
    }

    template <Ids OtherId>
    void fromMatrixLine(std::shared_ptr<MatrixLine<Type, OtherId>> line) {
        this->size(line->size());
        this->row(line->row());
        this->hostVectorValuePtr_ = line->vectorValue();
        this->hostLinePtr_ = line->get();
    }

    template <Ids OtherId>
    void fromCudaMatrixLine(std::shared_ptr<CudaMatrixLine<Type, OtherId>> line) {
        this->size(line->size());
        this->row(line->row());
        // save host ptrs
        this->hostVectorValuePtr_ = line->hostVectorValuePtr();
        this->hostLinePtr_ = line->hostLinePtr();
        // swap device memory
        Type *vec = line->vectorValue();
        Type *ptr = line->get();
        line->vectorValue(this->vectorValue());
        line->set(this->get());
        this->vectorValue(vec);
        this->set(ptr);
    }

    friend std::ostream &operator<<(std::ostream &os, CudaMatrixLine const &data) {
        os << "CudaMatrixLine " << Id << std::endl;
        os << "| row: " << data.row() << std::endl;
        os << "| size: " << data.size() << std::endl;
        os << "| ttl: " << data.ttl_ << std::endl;
        return os;
    }

  private:
    size_t ttl_ = 0;
    cudaEvent_t event_ = {};
    Type *hostVectorValuePtr_ = nullptr;
    Type *hostLinePtr_ = nullptr;
};

#endif
