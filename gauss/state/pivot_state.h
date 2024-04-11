#ifndef PIVOT_STATE_HPP
#define PIVOT_STATE_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include <memory>
#include "../data/matrix_line.h"
#include "../data/ids.h"

// Ensure that the first step of the Gauss pivot algorithm is done. At the end
// the result lines form a upper triangular matrix that we can use to solve the
// system.
template <typename Type>
class PivotState: public hh::AbstractState<
                            3,
                            MatrixLine<Type, Line>, MatrixLine<Type, DividedLine>, MatrixLine<Type, SubstractedLine>,
                            MatrixLine<Type, PivotLine>, MatrixLine<Type, Line>, MatrixLine<Type, ResultLine>> {
  public:
    PivotState(size_t nbLines):
        hh::AbstractState<
            3,
            MatrixLine<Type, Line>, MatrixLine<Type, DividedLine>, MatrixLine<Type, SubstractedLine>,
            MatrixLine<Type, PivotLine>, MatrixLine<Type, Line>, MatrixLine<Type, ResultLine>>(),
        totalNbLines_(nbLines) {}

    // treat lines from split matrix task
    void execute(std::shared_ptr<MatrixLine<Type, Line>> line) override {
        if (line->row() == currentPivotIdx_) {
            this->addResult(std::make_shared<MatrixLine<Type, PivotLine>>(line));
        } else {
            this->addResult(line);
        }
    }

    // we get the line to which with substracted the pivot line
    void execute(std::shared_ptr<MatrixLine<Type, SubstractedLine>> line) override {
        if (line->row() == currentPivotIdx_) {
            this->addResult(std::make_shared<MatrixLine<Type, PivotLine>>(line));
        } else if (line->row() > currentPivotIdx_) {
            this->addResult(std::make_shared<MatrixLine<Type, Line>>(line));
        }
    }

    // we get the pivot line with the pivot coef equal to 2. This line is
    // treated so it's sent as a result line.
    void execute(std::shared_ptr<MatrixLine<Type, DividedLine>> line) override {
        this->addResult(std::make_shared<MatrixLine<Type, ResultLine>>(line));
        ++currentPivotIdx_;
    }

    bool isDone() { return currentPivotIdx_ >= totalNbLines_; }

  private:
    size_t currentPivotIdx_ = 0;
    size_t totalNbLines_ = 0;
};

#endif
