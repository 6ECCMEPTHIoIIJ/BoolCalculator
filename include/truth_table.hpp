#ifndef MAIN_CPP_TRUTH_TABLE_HPP
#define MAIN_CPP_TRUTH_TABLE_HPP

#include <vector>
#include <iostream>

#include "reverse_polish_notation.hpp"

namespace BoolCalc {

  class TruthTable :
      public std::vector<std::vector<bool>> {
    uint8_t params_amount_;
    size_t operations_amount_;
  public:
    explicit TruthTable(const ReversePolishNotation& expr);

    [[nodiscard]] uint8_t params_amount() const;

    [[nodiscard]] size_t operations_amount() const;
  };
}

#endif //MAIN_CPP_TRUTH_TABLE_HPP
