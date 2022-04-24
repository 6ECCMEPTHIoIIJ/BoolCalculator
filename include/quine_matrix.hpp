#ifndef MAIN_CPP_QUINE_MATRIX_HPP
#define MAIN_CPP_QUINE_MATRIX_HPP

#include "cantor_abbreviated_normal_form.hpp"

namespace BoolCalc {

  class QuineMatrix {
    const CantorAbbreviatedNormalForm& implicants_;
    const CantorPerfectNormalForm& constituents_;
    std::vector<std::vector<bool>> overlap_table_;
    uint8_t params_amount_;
    size_t height_;
    size_t width_;
  public:
    QuineMatrix(const CantorAbbreviatedNormalForm& abbreviated,
                const CantorPerfectNormalForm& perfect);

    [[nodiscard]] CantorAbbreviatedNormalForm implicants() const;

    [[nodiscard]] CantorPerfectNormalForm constituents() const;

    [[nodiscard]] std::vector<std::vector<bool>> overlap_table() const;

    [[nodiscard]] uint8_t params_amount() const;

    [[nodiscard]] size_t height() const;

    [[nodiscard]] size_t width() const;
  };
}

#endif // MAIN_CPP_QUINE_MATRIX_HPP
