#include "data/matrix_data.h"
#include "data/vector_data.h"
#include "data/product_block_data.h"
#include "task/matrixproduct_task.h"
#include <cstddef>
#include <iostream>
#include <hedgehog/hedgehog.h>
#include <memory>

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
    constexpr size_t matrixWidth = 5;
    constexpr size_t matrixHeight = 5;
    constexpr size_t vectorSize = matrixWidth;
    constexpr size_t blockSize = 3;
    constexpr size_t numberThreads = 3;

    MatrixType *matrixData = createMatrix(matrixWidth, matrixHeight);
    MatrixType *vectorData = createVector(vectorSize);
    MatrixType *resultData = new MatrixType[vectorSize]();

    MatrixData<MatrixType> matrix(matrixWidth, matrixHeight, blockSize, matrixData);
    VectorData<MatrixType> vector(vectorSize, blockSize, vectorData);
    VectorData<MatrixType> result(vectorSize, blockSize, resultData);

    std::cout << "matrix:" << std::endl;
    std::cout << matrix << std::endl;
    std::cout << "vector:" << std::endl;
    std::cout << vector << std::endl;

    hh::Graph<1, ProductBlockData<MatrixType>, VectorBlockData<MatrixType>> graph("Matrix vector product");

    auto productTask = std::make_shared<MatrixProductTask<MatrixType>>("product", numberThreads);

    graph.inputs(productTask);
    graph.outputs(productTask);

    graph.executeGraph();

    size_t numberBlocksRows = matrix.getNumBlocksRows();
    size_t numberBlocksCols = matrix.getNumBlocksCols();

    for (size_t iGrid = 0; iGrid < numberBlocksRows; ++iGrid) {
        for (size_t jGrid = 0; jGrid < numberBlocksCols; ++jGrid) {
            auto product = std::make_shared<ProductBlockData<MatrixType>>();
            product->matrixBlockData = std::make_shared<MatrixBlockData<MatrixType>>(blockSize, iGrid * blockSize, jGrid * blockSize, matrix);
            product->vectorBlockData = std::make_shared<VectorBlockData<MatrixType>>(blockSize, jGrid * blockSize, vector);
            product->resultBlockData = std::make_shared<VectorBlockData<MatrixType>>(blockSize, iGrid * blockSize, result);
            graph.pushData(product);
        }
    }
    graph.finishPushingData();
    graph.waitForTermination();

    std::cout << result << std::endl;

    delete[] matrixData;
    delete[] vectorData;
    delete[] resultData;
    return 0;
}
