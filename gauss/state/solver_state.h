#ifndef SOLVER_STATE_HPP
#define SOLVER_STATE_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include "../data/ids.h"
#include "../data/matrix_line.h"

#define SolverStateInNb 2
#define SolverStateInput MatrixLine<Type, PivotedLine>, MatrixLine<Type, SubstractedLine>
#define SolverStateOutput MatrixLine<Type, PivotLine>, MatrixLine<Type, Line>, MatrixLine<Type, ResultLine>

template <typename Type>
class SolverState: public hh::AbstractState<SolverStateInNb, SolverStateInput, SolverStateOutput> {
  public:
    SolverState(size_t nbLines):
        hh::AbstractState<SolverStateInNb, SolverStateInput, SolverStateOutput>(),
        totalNbLines_(nbLines),
        currentPivotIdx_(nbLines - 1)
    {}

    void execute(std::shared_ptr<MatrixLine<Type, PivotedLine>> line) override {
        pivotedLines_.push_back(line);
        if (pivotedLines_.size() == totalNbLines_) {
            for (auto pivotedLine : pivotedLines_) {
                if (pivotedLine->row() == totalNbLines_ - 1) {
                    nbLinesTreated_ = totalNbLines_ - 1;
                    this->addResult(std::make_shared<MatrixLine<Type, PivotLine>>(pivotedLine));
                    this->addResult(std::make_shared<MatrixLine<Type, ResultLine>>(pivotedLine));
                    ++nbResultLines_;
                } else {
                    this->addResult(std::make_shared<MatrixLine<Type, Line>>(pivotedLine));
                }
            }
        }
    }

    void execute(std::shared_ptr<MatrixLine<Type, SubstractedLine>> line) override {
        --nbLinesTreated_;
        substractedLines_.push_back(line);

        if (nbLinesTreated_ == 0) {
            --currentPivotIdx_;
            nbLinesTreated_ = totalNbLines_ - nbResultLines_ - 1;
            for (auto subline : substractedLines_) {
                if (subline->row() == currentPivotIdx_) {
                    if (currentPivotIdx_ > 0) {
                        this->addResult(std::make_shared<MatrixLine<Type, PivotLine>>(subline));
                    }
                    this->addResult(std::make_shared<MatrixLine<Type, ResultLine>>(subline));
                    ++nbResultLines_;
                } else {
                    this->addResult(std::make_shared<MatrixLine<Type, Line>>(subline));
                }
            }
            substractedLines_.clear();
        }
    }

    bool isDone() { return nbResultLines_ == totalNbLines_; }

  private:
    size_t currentPivotIdx_ = 0;
    size_t totalNbLines_ = 0;
    size_t nbLinesTreated_ = 0;
    size_t nbResultLines_ = 0;
    std::vector<std::shared_ptr<MatrixLine<Type, SubstractedLine>>> substractedLines_ = {};
    std::vector<std::shared_ptr<MatrixLine<Type, PivotedLine>>> pivotedLines_ = {};
};

#endif
