#ifndef SUBSTRACT_LINES_STATE_HPP
#define SUBSTRACT_LINES_STATE_HPP

#include <cstddef>
#include <hedgehog/hedgehog.h>
#include <utility>
#include <vector>
#include <memory>
#include "../data/matrix_line.h"
#include "../data/ids.h"

template <typename Type>
using SubstractLinesOutType = std::pair<std::shared_ptr<MatrixLine<Type, PivotLine>>, std::shared_ptr<MatrixLine<Type, Line>>>;

#define SubstractLinesStateInNb 2
#define SubstractLinesStateInput MatrixLine<Type, PivotLine>, MatrixLine<Type, Line>
#define SubstractLinesStateOutput SubstractLinesOutType<Type>

// This state handle the substraction of the pivot lines to all the lines
// underneath.
template <typename Type>
class SubstractLinesState: public hh::AbstractState<SubstractLinesStateInNb, SubstractLinesStateInput, SubstractLinesStateOutput> {
  public:
    SubstractLinesState(size_t nbLines):
        hh::AbstractState<SubstractLinesStateInNb, SubstractLinesStateInput, SubstractLinesStateOutput>(),
        totalNbLines_(nbLines),
        nbLinesTreated_((nbLines * (nbLines - 1)) / 2) { }

    // on récupère la ligne du pivot
    void execute(std::shared_ptr<MatrixLine<Type, PivotLine>> pivotLine) override {
        this->pivotLine_ = pivotLine;
        ++currentLine_;
        for (auto line : lines_) {
            --nbLinesTreated_;
            this->addResult(std::make_shared<SubstractLinesOutType<Type>>(std::make_pair(
                        pivotLine_,
                        line
                        )));
        }
        lines_.clear();
    }

    // on récupère une ligne
    void execute(std::shared_ptr<MatrixLine<Type, Line>> line) override {
        if (pivotLine_) {
            --nbLinesTreated_;
            this->addResult(std::make_shared<SubstractLinesOutType<Type>>(std::make_pair(
                        pivotLine_,
                        line
                        )));
        } else {
            lines_.push_back(line);
        }
    }

    bool isDone() { return nbLinesTreated_ <= 0; }

  private:
    size_t totalNbLines_ = 0;
    size_t nbLinesTreated_ = 0;
    size_t currentLine_ = 0;
    std::vector<std::shared_ptr<MatrixLine<Type, Line>>> lines_ = {};
    std::shared_ptr<MatrixLine<Type, PivotLine>> pivotLine_ = nullptr;
};

#endif
