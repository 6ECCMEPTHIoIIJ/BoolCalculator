#include "cantor_abbreviated_normal_form.hpp"

namespace BoolCalc {

  CantorAbbreviatedNormalForm::CantorAbbreviatedNormalForm(const GluingTable& table) {
    params_amount_ = table.params_amount();
    for (auto& row: table) {
      for (auto& column: row) {
        for (auto& c: column) {
          if (!c.is_in_use()) {
            push_back(c);
          }
        }
      }
    }
  }

  uint8_t CantorAbbreviatedNormalForm::params_amount() const {
    return params_amount_;
  }
}