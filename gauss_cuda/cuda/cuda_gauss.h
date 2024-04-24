#ifndef CUDA_GAUSS_HPP
#define CUDA_GAUSS_HPP
#include <cstddef>
#include <cuda.h>
#include <driver_types.h>

void substractLines(double *res, double *pivot, double *resVector,
        double *pivotVector, size_t coef, size_t size, cudaStream_t stream);

#endif
