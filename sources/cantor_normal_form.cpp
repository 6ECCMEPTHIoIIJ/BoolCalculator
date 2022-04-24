#include "cantor_normal_form.hpp"

namespace BoolCalc {

  CantorNormalForm::CantorNormalForm(const ReversePolishNotation& expr) :
      params_amount_(expr.params_amount()),
      truth_table_(expr),
      perfect_(truth_table_),
      gluing_table_(perfect_),
      abbreviated_(gluing_table_),
      quine_matrix_(abbreviated_,
                    perfect_),
      quine_core_(quine_matrix_) {
  }

  TruthTable CantorNormalForm::truth_table() const {
    return truth_table_;
  }

  GluingTable CantorNormalForm::gluing_table() const {
    return gluing_table_;
  }

  CantorPerfectNormalForm CantorNormalForm::perfect() const {
    return perfect_;
  }

  CantorAbbreviatedNormalForm CantorNormalForm::abbreviated() const {
    return abbreviated_;
  }

  uint8_t CantorNormalForm::params_amount() const {
    return params_amount_;
  }

  QuineMatrix CantorNormalForm::quine_matrix() const {
    return quine_matrix_;
  }

  QuineCore CantorNormalForm::quine_core() const {
    return quine_core_;
  }
}