#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <random>
#include <iostream>
#include "data/matrix.h"

/******************************************************************************/
/*                                 structures                                 */
/******************************************************************************/

struct Config {
    size_t matrixSize;
    size_t nbThreads;
    bool printMatrix;
    bool printResult;
    std::string dotDirectory;
};

/******************************************************************************/
/*                                 functions                                  */
/******************************************************************************/

void parseCmdArgs(int argc, char **argv, Config &config);

template <typename Type>
void generateRandomProblem(size_t nbVariables, Type *matrix, Type *vector, Type *variables) {
    std::random_device dv;
    std::mt19937 gen(dv());
    std::uniform_real_distribution<> dis(-100, 100);
    size_t width = nbVariables, height = nbVariables;

    for (size_t i = 0; i < nbVariables; ++i) {
        variables[i] = dis(gen);
    }

    for (size_t i = 0; i < height; ++i) {
        Type sum = 0;
        for (size_t j = 0; j < width; ++j) {
            Type value = dis(gen);
            if (i == j && value == 0) value++; // make sure we don't have 0 on
                                               // the diagonal (normally, we should
                                               // exchange lines in this case
                                               // but it's not done yet in the
                                               // graph)
            matrix[i * width + j] = value;
            sum += value * variables[j];
        }
        vector[i] = sum;
    }
}

template <typename Type>
bool isIdentity(Matrix<Type> const& matrix, Type precision) {
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

template <typename Type>
bool isTriangular(Matrix<Type> const& matrix, Type precision) {
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

template <typename Type>
bool verrifySolution(size_t nbVariables, Type *founded, Type *expected, Type precision) {
    bool output = true;

    for (size_t i = 0; i < nbVariables; ++i) {
        if (!((founded[i] - precision) <= expected[i] && expected[i] <= (founded[i] + precision))) {
            output = false;
            std::cout << "Error: " << expected[i] << " != " << founded[i] << std::endl;
        }
    }
    return output;
}

#endif
