#ifndef MAIN_CPP_GLUING_TABLE_HPP
#define MAIN_CPP_GLUING_TABLE_HPP

#include <vector>

#include "cantor_perfect_normal_form.hpp"

namespace BoolCalc {

  class GluingTable :
      public std::vector<std::vector<std::vector<Constituent>>> {
    uint8_t params_amount_;

    static void removeDuplicates(std::vector<Constituent>& column);

    void glue();

  public:
    explicit GluingTable(const CantorPerfectNormalForm& cpnf);

    [[nodiscard]] uint8_t params_amount() const;
  };
}

#endif //MAIN_CPP_GLUING_TABLE_HPP
