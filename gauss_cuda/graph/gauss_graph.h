#ifndef GAUSS_GRAPH_HPP
#define GAUSS_GRAPH_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include "../data/matrix.h"
#include "../data/matrix_line.h"
#include "../data/ids.h"
#include "gauss_pivot_graph.h"
#include "gauss_solver_graph.h"

#define GaussGraphInNb 1
#define GaussGraphInput std::pair<Matrix<Type>, Vector<Type>>
#define GaussGraphOutput MatrixLine<Type, ResultLine>

template <typename Type>
class GaussGraph: public hh::Graph<GaussGraphInNb, GaussGraphInput, GaussGraphOutput> {
  public:
    GaussGraph(size_t matrixHeight, size_t nbThreads):
        hh::Graph<GaussGraphInNb, GaussGraphInput, GaussGraphOutput>("Gauss graph")
    {
        auto pivotGraph = std::make_shared<GaussPivotGraph<Type>>(matrixHeight, nbThreads);
        auto solverGraph = std::make_shared<GaussSolverGraph<Type>>(matrixHeight, nbThreads);

        this->inputs(pivotGraph);
        this->edges(pivotGraph, solverGraph);
        this->outputs(solverGraph);
    }
};

#endif
