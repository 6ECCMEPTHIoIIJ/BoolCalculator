#include "truth_table.hpp"

namespace BoolCalc {

  TruthTable::TruthTable(const ReversePolishNotation& expr) {
    params_amount_ = expr.params_amount();
    resize(1 << params_amount_);
    const size_t max_value = 1 << params_amount_;
    for (size_t value = 0; value < max_value; value++) {
      auto operations_results = expr.calculate(value);
      operations_amount_ = operations_results.size();
      at(value).reserve(operations_amount_);
      for (const auto& operation_result: operations_results) {
        at(value).push_back(operation_result);
      }
    }
  }

  uint8_t TruthTable::params_amount() const {
    return params_amount_;
  }

  size_t TruthTable::operations_amount() const {
    return operations_amount_;
  }
}