#include "cuda_gauss.h"
#include <cstdio>

__global__ void substractLines_impl(double *res, double *pivot, double *resVector,
        double *pivotVector, double coef, size_t size) {
    size_t i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < size) {
        res[i] -= pivot[i] * coef;
    }
    if (i == 0) {
        *resVector -= *pivotVector * coef;
    }
}

void substractLines(double *res, double *pivot, double *resVector,
        double *pivotVector, double coef, size_t size, cudaStream_t stream)
{
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    substractLines_impl<<<blocksPerGrid, threadsPerBlock, 0, stream>>>(
            res, pivot, resVector, pivotVector, coef, size);
}
