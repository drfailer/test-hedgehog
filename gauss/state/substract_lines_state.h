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
        nbLinesToTreatForCurrentPivot_(nbLines - 1),
        nbLinesToTreat_((nbLines * (nbLines - 1)) / 2)
        { }

    // on récupère la ligne du pivot
    void execute(std::shared_ptr<MatrixLine<Type, PivotLine>> pivotLine) override {
        this->pivotLine_ = pivotLine;
        ++currentPivot_;
        nbLinesToTreatForCurrentPivot_ = totalNbLines_ - currentPivot_ - lines_.size();
        for (auto line : lines_) {
            this->addResult(std::make_shared<SubstractLinesOutType<Type>>(pivotLine_, line));
            --nbLinesToTreat_;
        }
        lines_.clear();

        if (nbLinesToTreatForCurrentPivot_ == 0) {
            pivotLine_ = nullptr;
        }
    }

    // on récupère une ligne
    void execute(std::shared_ptr<MatrixLine<Type, Line>> line) override {
        if (pivotLine_) {
            this->addResult(std::make_shared<SubstractLinesOutType<Type>>(pivotLine_, line));
            --nbLinesToTreat_;
            --nbLinesToTreatForCurrentPivot_;
            if (nbLinesToTreatForCurrentPivot_ == 0) {
                pivotLine_ = nullptr;
            }
        } else {
            lines_.emplace_back(line);
        }
    }

    bool isDone() { return nbLinesToTreat_ <= 0; }

  private:
    size_t totalNbLines_ = 0;
    size_t nbLinesToTreat_ = 0;
    size_t nbLinesToTreatForCurrentPivot_ = 0;
    size_t currentPivot_ = 0;
    std::vector<std::shared_ptr<MatrixLine<Type, Line>>> lines_ = {};
    std::shared_ptr<MatrixLine<Type, PivotLine>> pivotLine_ = nullptr;
};

#endif
