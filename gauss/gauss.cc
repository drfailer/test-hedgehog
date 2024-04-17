#include <format>
#include <iostream>
#include <filesystem>

#include "data/matrix.h"
#include "data/vector.h"
#include "graph/gauss_graph.h"
#include "utils.h"

using MatrixType = double;

int main(int argc, char** argv) {
    Config config;

    parseCmdArgs(argc, argv, config);

    MatrixType *matrixMem = new MatrixType[config.matrixSize * config.matrixSize]();
    MatrixType *vectorMem = new MatrixType[config.matrixSize]();
    MatrixType *variablesMem = new MatrixType[config.matrixSize]();

    generateRandomProblem(config.matrixSize, matrixMem, vectorMem, variablesMem);

    if (!std::filesystem::exists(config.dotDirectory)) {
        std::filesystem::create_directory(config.dotDirectory);
    }

    Matrix<MatrixType> matrix(config.matrixSize, config.matrixSize, matrixMem);
    Vector<MatrixType> vector(config.matrixSize, vectorMem);
    auto input = std::make_shared<std::pair<Matrix<MatrixType>, Vector<MatrixType>>>(matrix, vector);

    if (config.printMatrix) {
        std::cout << matrix << std::endl;
        std::cout << vector << std::endl;
    }

    GaussGraph<MatrixType> gaussGraph(config.matrixSize, config.nbThreads);

    gaussGraph.executeGraph();
    gaussGraph.pushData(input);
    gaussGraph.finishPushingData();
    gaussGraph.waitForTermination();
    gaussGraph.createDotFile(std::format("{}/gaussGraph{}-{}.dot", config.dotDirectory, config.nbThreads, config.matrixSize), hh::ColorScheme::EXECUTION, hh::StructureOptions::QUEUE);

    assert(isIdentity(matrix, 1e-3));
    assert(isTriangular(matrix, 1e-3));
    assert(verrifySolution(config.matrixSize, vector.get(), variablesMem, 1e-3));

    if (config.printResult) {
        std::cout << vector << std::endl;
    }

    delete[] matrixMem;
    delete[] vectorMem;
    delete[] variablesMem;
    return 0;
}
