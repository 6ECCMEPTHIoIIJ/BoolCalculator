#ifndef MAIN_CPP_QUINE_CORE_HPP
#define MAIN_CPP_QUINE_CORE_HPP

#include "quine_matrix.hpp"

namespace BoolCalc {

  class QuineCore {
    const QuineMatrix& matrix_;
    std::vector<bool> core_;
    size_t size_;
  public:
    explicit QuineCore(const QuineMatrix& matrix);

    [[nodiscard]]  std::vector<Constituent> implicants() const;

    [[nodiscard]]  std::vector<bool> core() const;

    [[nodiscard]]  size_t size() const;
  };
}

#endif // MAIN_CPP_QUINE_CORE_HPP
