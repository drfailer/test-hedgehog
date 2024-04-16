#include <iostream>
#include <memory>
#include <utility>
#include <random>

#include "data/matrix.h"
#include "data/vector.h"
#include "graph/gauss_pivot_graph.h"

using MatrixType = double;

void generateRandomProblem(size_t nbVariables, MatrixType *matrix, MatrixType *vector, MatrixType *variables) {
    std::random_device dv;
    std::mt19937 gen(dv());
    std::uniform_int_distribution<> dis(-10, 10);
    size_t width = nbVariables, height = nbVariables;

    for (size_t i = 0; i < nbVariables; ++i) {
        variables[i] = dis(gen);
    }

    for (size_t i = 0; i < height; ++i) {
        MatrixType sum = 0;
        for (size_t j = 0; j < width; ++j) {
            MatrixType value = dis(gen);
            matrix[i * width + j] = value;
            sum += value * variables[j];
        }
        vector[i] = sum;
    }
}

bool isIdentity(Matrix<MatrixType> const& matrix, MatrixType precision) {
    for (size_t i = 0; i < matrix.height(); ++i) {
        for (size_t j = 0; j < matrix.width(); ++j) {
            bool isOne = (1 - precision) <= matrix.get()[i * matrix.width() + j]
                      && matrix.get()[i * matrix.width() + j] <= (1 + precision);
            bool isZero = -precision <= matrix.get()[i * matrix.width() + j]
                       && matrix.get()[i * matrix.width() + j] <= precision;
            if ((i == j && !isOne) || (i != j && !isZero)) {
                    std::cout << i << ", " << j << ": " << matrix.get()[i * matrix.width() + j] << std::endl;
                    return false;
            }
        }
    }
    return true;
}

bool isTriangular(Matrix<MatrixType> const& matrix, MatrixType precision) {
    for (size_t i = 0; i < matrix.height(); ++i) {
        for (size_t j = 0; j <= i; ++j) {
            bool isOne = (1 - precision) <= matrix.get()[i * matrix.width() + j]
                      && matrix.get()[i * matrix.width() + j] <= (1 + precision);
            bool isZero = -precision <= matrix.get()[i * matrix.width() + j]
                       && matrix.get()[i * matrix.width() + j] <= precision;
            if ((i == j && !isOne) || (i != j && !isZero)) {
                    std::cout << i << ", " << j << ": " << matrix.get()[i * matrix.width() + j] << std::endl;
                    return false;
            }
        }
    }
    return true;
}

bool verrifySolution(size_t nbVariables, MatrixType *founded, MatrixType *expected, MatrixType precision) {
    bool output = true;

    for (size_t i = 0; i < nbVariables; ++i) {
        if (!((founded[i] - precision) <= expected[i] && expected[i] <= (founded[i] + precision))) {
            output = false;
            std::cout << "Error: " << expected[i] << " != " << founded[i] << std::endl;
        }
    }
    return output;
}

/* test matrix for debugging */
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
    constexpr size_t matrixWidth = 100;
    constexpr size_t matrixHeight = 100;
    constexpr size_t nbThreads = 3;

    MatrixType *matrixMem = new MatrixType[matrixWidth * matrixHeight]();
    MatrixType *vectorMem = new MatrixType[matrixHeight]();
    MatrixType *variablesMem = new MatrixType[matrixHeight]();

    generateRandomProblem(matrixHeight, matrixMem, vectorMem, variablesMem);

    Matrix<MatrixType> matrix(matrixWidth, matrixHeight, matrixMem);
    Vector<MatrixType> vector(matrixHeight, vectorMem);
    auto input = std::make_shared<std::pair<Matrix<MatrixType>, Vector<MatrixType>>>(matrix, vector);
    /* std::cout << matrix << std::endl; */
    /* std::cout << vector << std::endl; */

    GaussPivotGraph<MatrixType> pivotGraph(matrixHeight, nbThreads);

    pivotGraph.executeGraph();
    pivotGraph.pushData(input);
    pivotGraph.finishPushingData();
    pivotGraph.waitForTermination();

    /* std::cout << matrix << std::endl; */
    /* std::cout << vector << std::endl; */

    pivotGraph.createDotFile("gausPivot.dot", hh::ColorScheme::EXECUTION, hh::StructureOptions::ALL);

    /* assert(isIdentity(matrix, 1e-3)); */
    assert(isTriangular(matrix, 1e-3));
    /* assert(verrifySolution(matrixHeight, vector.get(), variablesMem, 1e-3)); */

    delete[] matrixMem;
    delete[] vectorMem;
    delete[] variablesMem;
    return 0;
}
