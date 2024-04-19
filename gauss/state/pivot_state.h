#ifndef PIVOT_STATE_HPP
#define PIVOT_STATE_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include <memory>
#include <vector>
#include "../data/matrix_line.h"
#include "../data/ids.h"

#define PivotStateInNb 3
#define PivotStateInput MatrixLine<Type, Line>, MatrixLine<Type, PivotedLine>, MatrixLine<Type, SubstractedLine>
#define PivotStateOutput MatrixLine<Type, BasePivotLine>, MatrixLine<Type, Line>, MatrixLine<Type, PivotedLine>

// Ensure that the first step of the Gauss pivot algorithm is done. At the end
// the result lines form a upper triangular matrix that we can use to solve the
// system.
template <typename Type>
class PivotState: public hh::AbstractState<PivotStateInNb, PivotStateInput, PivotStateOutput> {
  public:
    PivotState(size_t nbLines):
        hh::AbstractState<PivotStateInNb, PivotStateInput, PivotStateOutput>(),
        totalNbLines_(nbLines), nbLinesToTreat_(nbLines - 1) {}

    // treat lines from split matrix task
    void execute(std::shared_ptr<MatrixLine<Type, Line>> line) override {
        if (line->row() == 0) {
            this->addResult(std::make_shared<MatrixLine<Type, BasePivotLine>>(line));
        } else {
            this->addResult(line);
        }
    }

    // we get the line to which with substracted the pivot line
    void execute(std::shared_ptr<MatrixLine<Type, SubstractedLine>> line) override {
        --nbLinesToTreat_;
        substractedLines_.emplace_back(line);

        if (nbLinesToTreat_ == 0) {
            ++currentPivotIdx_;
            nbLinesToTreat_ = totalNbLines_ - currentPivotIdx_ - 1;
            for (auto subline : substractedLines_) {
                if (subline->row() == currentPivotIdx_) {
                    this->addResult(std::make_shared<MatrixLine<Type, BasePivotLine>>(subline));
                } else {
                    this->addResult(std::make_shared<MatrixLine<Type, Line>>(subline));
                }
            }
            substractedLines_.clear();
        }
    }

    // we get the pivot line with the pivot coef equal to 2. This line is
    // treated so it's sent as a result line.
    void execute(std::shared_ptr<MatrixLine<Type, PivotedLine>> line) override {
        this->addResult(line);
        ++nbPivotedLines_;
    }

    bool isDone() { return nbPivotedLines_ == totalNbLines_; }

  private:
    size_t currentPivotIdx_ = 0;
    size_t totalNbLines_ = 0;
    size_t nbLinesToTreat_ = 0;
    size_t nbPivotedLines_ = 0;
    std::vector<std::shared_ptr<MatrixLine<Type, SubstractedLine>>> substractedLines_ = {};
};

#endif
