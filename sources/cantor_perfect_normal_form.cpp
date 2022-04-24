#include  "cantor_perfect_normal_form.hpp"

namespace BoolCalc {

  CantorPerfectNormalForm::CantorPerfectNormalForm(const TruthTable& table) {
    params_amount_ = table.params_amount();
    for (size_t value = 0; value < table.size(); value++) {
      if (table[value].back()) {
        push_back(Constituent(value, params_amount_));
      }
    }
  }

  uint8_t CantorPerfectNormalForm::params_amount() const {
    return params_amount_;
  }

  CantorPerfectNormalForm::~CantorPerfectNormalForm() = default;
}