#include "data/matrix_data.h"
#include "data/vector_data.h"
#include "data/product_block_data.h"
#include "task/matrixproduct_task.h"
#include "state/block_state.h"
#include "task/split_matrix_task.h"
#include "task/split_vector_task.h"
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

    auto matrix = std::make_shared<MatrixData<MatrixType, Matrix>>(matrixWidth, matrixHeight, blockSize, matrixData);
    auto vector = std::make_shared<VectorData<MatrixType, Vector>>(matrixWidth, matrixHeight, blockSize, vectorData);
    auto result = std::make_shared<VectorData<MatrixType, Result>>(matrixWidth, matrixHeight, blockSize, resultData);

    std::cout << "matrix:" << std::endl;
    std::cout << *matrix << std::endl;
    std::cout << "vector:" << std::endl;
    std::cout << *vector << std::endl;

    hh::Graph<3,
        MatrixData<MatrixType, Matrix>,
        VectorData<MatrixType, Vector>,
        VectorData<MatrixType, Result>,
        VectorBlockData<MatrixType, Result>> graph("Matrix vector product");

    auto splitMatrix = std::make_shared<SplitMatrixTask<MatrixType, Matrix>>();
    auto splitVector = std::make_shared<SplitVectorTask<MatrixType, Vector>>();
    auto splitResult = std::make_shared<SplitVectorTask<MatrixType, Result>>();
    auto productTask = std::make_shared<MatrixProductTask<MatrixType>>("product", numberThreads);

    auto inputState = std::make_shared<BlockState<MatrixType>>(matrix->getNumBlocksRows(), matrix->getNumBlocksCols(), matrix->getBlockSize());
    auto inputstateManager =
        std::make_shared<
            hh::StateManager<3,
                MatrixBlockData<MatrixType, Matrix>,
                VectorBlockData<MatrixType, Vector>,
                VectorBlockData<MatrixType, Result>,
                ProductBlockData<MatrixType>>>(inputState, "Block State Manager");


    graph.inputs(splitMatrix);
    graph.inputs(splitVector);
    graph.inputs(splitResult);

    // link split tasks
    graph.edges(splitMatrix, inputstateManager);
    graph.edges(splitVector, inputstateManager);
    graph.edges(splitResult, inputstateManager);

    // link product task
    graph.edges(inputstateManager, productTask);

    graph.outputs(productTask);

    graph.executeGraph();

    graph.pushData(matrix);
    graph.pushData(vector);
    graph.pushData(result);

    graph.finishPushingData();
    graph.waitForTermination();

    std::cout << *result << std::endl;

    delete[] matrixData;
    delete[] vectorData;
    delete[] resultData;
    return 0;
}
