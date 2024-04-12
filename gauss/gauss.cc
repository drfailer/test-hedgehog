#include <iostream>
#include <memory>
#include <utility>

#include "data/matrix.h"
#include "data/vector.h"
#include "graph/gauss_pivot_graph.h"

using MatrixType = double;

MatrixType* setupMatrix(size_t width, size_t height) {
    MatrixType *matrixMem = new MatrixType[width * height]();

    matrixMem[0] = 1;
    matrixMem[1] = 2;
    matrixMem[2] = -2;
    matrixMem[3] = 2;
    matrixMem[4] = 1;
    matrixMem[5] = -5;
    matrixMem[6] = 1;
    matrixMem[7] = -4;
    matrixMem[8] = 1;

    // matrix for test
    return matrixMem;
}

MatrixType* setupVector(size_t size) {
    MatrixType *vectorMem = new MatrixType[size]();

    vectorMem[0] = -15;
    vectorMem[1] = -21;
    vectorMem[2] = 18;

    // vector for test
    return vectorMem;
}

int main(int, char**) {
    constexpr size_t matrixWidth = 3;
    constexpr size_t matrixHeight = 3;
    constexpr size_t nbThreads = 2;

    static_assert(nbThreads < matrixHeight, "nbThreads should be infirior to the number of equations.");

    MatrixType *matrixMem = setupMatrix(matrixWidth, matrixHeight);
    MatrixType *vectorMem = setupVector(matrixHeight);

    Matrix<MatrixType> matrix(matrixWidth, matrixHeight, matrixMem);
    Vector<MatrixType> vector(matrixHeight, vectorMem);
    auto input = std::make_shared<std::pair<Matrix<MatrixType>, Vector<MatrixType>>>(matrix, vector);
    std::cout << matrix << std::endl;
    std::cout << vector << std::endl;

    GaussPivotGraph<MatrixType> pivotGraph(matrixHeight, nbThreads);

    pivotGraph.executeGraph();
    pivotGraph.pushData(input);
    pivotGraph.finishPushingData();
    pivotGraph.waitForTermination();

    std::cout << matrix << std::endl;
    std::cout << vector << std::endl;

    pivotGraph.createDotFile("gausPivot.dot", hh::ColorScheme::EXECUTION, hh::StructureOptions::ALL);

    delete[] matrixMem;
    delete[] vectorMem;
    return 0;
}
