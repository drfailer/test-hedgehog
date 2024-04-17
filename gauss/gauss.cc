#include <iostream>
#include <memory>
#include <utility>

#include "data/matrix.h"
#include "data/vector.h"
#include "graph/gauss_graph.h"
#include "utils.h"

using MatrixType = double;

int main(int, char**) {
    constexpr size_t matrixSize = 1000;
    constexpr size_t nbThreads = 4;
    constexpr bool printMatrix = false;
    constexpr bool printSolution = false;

    MatrixType *matrixMem = new MatrixType[matrixSize * matrixSize]();
    MatrixType *vectorMem = new MatrixType[matrixSize]();
    MatrixType *variablesMem = new MatrixType[matrixSize]();

    generateRandomProblem(matrixSize, matrixMem, vectorMem, variablesMem);

    Matrix<MatrixType> matrix(matrixSize, matrixSize, matrixMem);
    Vector<MatrixType> vector(matrixSize, vectorMem);
    auto input = std::make_shared<std::pair<Matrix<MatrixType>, Vector<MatrixType>>>(matrix, vector);

    if (printMatrix) {
        std::cout << matrix << std::endl;
        std::cout << vector << std::endl;
    }

    GaussGraph<MatrixType> gaussGraph(matrixSize, nbThreads);

    gaussGraph.executeGraph();
    gaussGraph.pushData(input);
    gaussGraph.finishPushingData();
    gaussGraph.waitForTermination();
    gaussGraph.createDotFile("gausPivot.dot", hh::ColorScheme::EXECUTION, hh::StructureOptions::QUEUE);

    assert(isIdentity(matrix, 1e-3));
    assert(isTriangular(matrix, 1e-3));
    assert(verrifySolution(matrixSize, vector.get(), variablesMem, 1e-3));

    if (printSolution) {
        std::cout << vector << std::endl;
    }

    delete[] matrixMem;
    delete[] vectorMem;
    delete[] variablesMem;
    return 0;
}
