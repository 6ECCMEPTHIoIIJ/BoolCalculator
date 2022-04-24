#ifndef MAIN_CPP_CANTOR_ABBREVIATED_NORMAL_FORM_HPP
#define MAIN_CPP_CANTOR_ABBREVIATED_NORMAL_FORM_HPP

#include <vector>
#include "gluing_table.hpp"

namespace BoolCalc {

  class CantorAbbreviatedNormalForm :
      public std::vector<Constituent> {
    uint8_t params_amount_;
  public:
    explicit CantorAbbreviatedNormalForm(const GluingTable& table);

    [[nodiscard]] uint8_t params_amount() const;
  };
}

#endif // MAIN_CPP_CANTOR_ABBREVIATED_NORMAL_FORM_HPP
