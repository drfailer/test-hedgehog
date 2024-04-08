#include "data/matrix_data.h"
#include "data/vector_data.h"
#include <cstddef>
#include <iostream>

using MatrixType = int;

MatrixType *createMatrix(size_t width, size_t height) {
    MatrixType *data = new MatrixType[width * height]();

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            data[i * width + j] = i * width + j + 1;
        }
    }
    return data;
}

MatrixType *createVector(size_t size) {
    MatrixType *data = new MatrixType[size]();

    for (size_t i = 0; i < size; ++i) {
        data[i] = i + 1;
    }
    return data;
}

int main(int, char**) {
    constexpr size_t matrixWidth = 2;
    constexpr size_t matrixHeight = 2;
    constexpr size_t vectorSize = matrixWidth;
    constexpr size_t blockSize = 1;

    MatrixType *matrixData = createMatrix(matrixWidth, matrixHeight);
    MatrixType *vectorData = createVector(vectorSize);
    MatrixType *resultData = new MatrixType[vectorSize]();

    MatrixData<MatrixType> matrix(matrixWidth, matrixHeight, blockSize, matrixData);
    VectorData<MatrixType> vector(vectorSize, blockSize, vectorData);
    VectorData<MatrixType> result(vectorSize, blockSize, resultData);

    std::cout << "matrix:" << std::endl;
    std::cout << matrix << std::endl;
    std::cout << vector << std::endl;
    std::cout << "vector:" << std::endl;

    delete[] matrixData;
    delete[] vectorData;
    delete[] resultData;
    return 0;
}
