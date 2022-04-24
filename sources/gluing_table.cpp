#include "gluing_table.hpp"

#include <sstream>
#include <iomanip>

namespace BoolCalc {
  GluingTable::GluingTable(const CantorPerfectNormalForm& cpnf) :
      std::vector<std::vector<std::vector<Constituent>>>(1) {
    params_amount_ = cpnf.params_amount();
    data()->resize(params_amount_ + 1);
    const uint16_t mask = (1 << params_amount_) - 1;
    for (const auto& c: cpnf) {
      uint16_t t = c.value() & mask;
      uint8_t truth_counter = 0;
      while (t) {
        truth_counter += t & 1;
        t >>= 1;
      }
      at(0).at(truth_counter).push_back(c);
    }
    glue();
  }

  void GluingTable::removeDuplicates(std::vector<Constituent>& column) {
    if (column.empty()) {
      return;
    }

    for (auto c_1 = column.begin(); c_1 != column.end() - 1 && c_1 != column.end(); c_1++) {
      for (auto c_2 = c_1 + 1; c_2 != column.end();) {
        if ((*c_1).isEqualTo(*c_2)) {
          c_2 = column.erase(c_2);
        } else {
          c_2++;
        }
      }
    }
  }

  void GluingTable::glue() {
    uint8_t gluing_depth = 0;
    std::vector<std::vector<Constituent>> tmp_row(params_amount_ + 1);
    std::vector<Constituent> tmp_column;
    do {
      if (gluing_depth > 0) {
        for (auto& column: at(gluing_depth)) {
          removeDuplicates(column);
        }
      }

      bool need_to_push_back = false;
      tmp_row.clear();
      for (auto column = at(gluing_depth).begin() + 1; column != at(gluing_depth).end(); column++) {
        tmp_column.clear();
        if (!(column - 1)->empty() && !column->empty()) {
          for (auto& c_1: *(column - 1)) {
            for (auto& c_2: *column) {
              if (c_1.canBeGluedWith(c_2)) {
                tmp_column.emplace_back(Constituent(c_1, c_2));
                need_to_push_back = true;
              }
            }
          }
        }
        tmp_row.emplace_back(tmp_column);
      }
      if (need_to_push_back) {
        tmp_row.resize(params_amount_ + 1);
        push_back(tmp_row);
      }
    } while (size() == ++gluing_depth + 1);
  }

  uint8_t GluingTable::params_amount() const {
    return params_amount_;
  }
}