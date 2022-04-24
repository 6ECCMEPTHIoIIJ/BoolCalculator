#ifndef MAIN_CPP_CANTOR_NORMAL_FORM_HPP
#define MAIN_CPP_CANTOR_NORMAL_FORM_HPP

#include "quine_core.hpp"

namespace BoolCalc {

  class CantorNormalForm {
    TruthTable truth_table_;
    CantorPerfectNormalForm perfect_;
    GluingTable gluing_table_;
    CantorAbbreviatedNormalForm abbreviated_;
    QuineMatrix quine_matrix_;
    QuineCore quine_core_;
    uint8_t params_amount_;
  public:
    explicit CantorNormalForm(const ReversePolishNotation& expr);

    [[nodiscard]] TruthTable truth_table() const;

    [[nodiscard]] GluingTable gluing_table() const;

    [[nodiscard]] CantorPerfectNormalForm perfect() const;

    [[nodiscard]] CantorAbbreviatedNormalForm abbreviated() const;

    [[nodiscard]] QuineMatrix quine_matrix() const;

    [[nodiscard]] QuineCore quine_core() const;

    [[nodiscard]] uint8_t params_amount() const;
  };
}

#endif //MAIN_CPP_CANTOR_NORMAL_FORM_HPP
