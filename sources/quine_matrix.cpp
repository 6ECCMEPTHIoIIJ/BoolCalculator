#include "quine_matrix.hpp"

#include <utility>

namespace BoolCalc {

  QuineMatrix::QuineMatrix(const CantorAbbreviatedNormalForm&  abbreviated,
                           const CantorPerfectNormalForm& perfect) :
      implicants_(abbreviated),
      constituents_(perfect) {
    params_amount_ = constituents_.params_amount();
    height_ = implicants_.size();
    width_ = constituents_.size();
    overlap_table_.resize(height_);
    for (size_t h = 0; h < height_; h++) {
      for (size_t w = 0; w < width_; w++) {
        overlap_table_[h].push_back(constituents_[w].isCoatingBy(implicants_[h]));
      }
    }
  }

  CantorAbbreviatedNormalForm QuineMatrix::implicants() const {
    return implicants_;
  }

  CantorPerfectNormalForm QuineMatrix::constituents() const {
    return constituents_;
  }

  std::vector<std::vector<bool>> QuineMatrix::overlap_table() const {
    return overlap_table_;
  }

  uint8_t QuineMatrix::params_amount() const {
    return params_amount_;
  }

  size_t QuineMatrix::height() const {
    return height_;
  }

  size_t QuineMatrix::width() const {
    return width_;
  }
}