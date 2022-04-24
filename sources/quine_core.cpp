#include "quine_core.hpp"

namespace BoolCalc {

  QuineCore::QuineCore(const QuineMatrix& matrix) :
      matrix_(matrix),
      size_(matrix.height()),
      core_(matrix.height(), false) {
    std::vector<bool> constituents_covered_by_one_implicant(matrix.implicants().size());
    for (size_t w = 0; w < matrix.width(); w++) {
      size_t cover_counter = 0;
      for (size_t h = 0; h < matrix.height(); h++) {
        cover_counter += matrix.overlap_table()[h][w];
      }
      constituents_covered_by_one_implicant[w] = cover_counter == 1;
    }

    for (size_t h = 0; h < matrix.height(); h++) {
      for (size_t w = 0; w < matrix.width(); w++) {
        if (matrix.overlap_table()[h][w] && constituents_covered_by_one_implicant[w]) {
          core_[h] = true;
          break;
        }
      }
    }
  }

  std::vector<Constituent> QuineCore::implicants() const {
    return matrix_.implicants();
  }

  std::vector<bool> QuineCore::core() const {
    return core_;
  }

  size_t QuineCore::size() const {
    return size_;
  }
}