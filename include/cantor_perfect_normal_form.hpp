#ifndef MAIN_CPP_CANTOR_PERFECT_NORMAL_FORM_HPP
#define MAIN_CPP_CANTOR_PERFECT_NORMAL_FORM_HPP

#include <vector>
#include <functional>

#include "constituent.hpp"
#include "truth_table.hpp"

namespace BoolCalc {

  class CantorPerfectNormalForm :
      public std::vector<Constituent> {
    uint8_t params_amount_;
  public:
    explicit CantorPerfectNormalForm(const TruthTable& table);

    [[nodiscard]] uint8_t params_amount() const;

    ~CantorPerfectNormalForm();
  };
}

#endif // MAIN_CPP_CANTOR_PERFECT_NORMAL_FORM_HPP
