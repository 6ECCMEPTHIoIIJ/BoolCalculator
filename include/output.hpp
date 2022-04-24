#ifndef MAIN_CPP_OUTPUT_HPP
#define MAIN_CPP_OUTPUT_HPP

#include <iostream>
#include <sstream>
#include <iomanip>

#include "cantor_normal_form.hpp"

namespace BoolCalc {

  namespace Output {

    enum class Mode {
      CANTOR,
      BOOLEAN
    };

    void cantor();

    void boolean();

    Mode getMode();
  }

  std::ostream& operator<<(std::ostream& out,
                           const TruthTable& table);

  std::ostream& operator<<(std::ostream& out,
                           const QuineMatrix& matrix);

  std::ostream& operator<<(std::ostream& out,
                           const QuineCore& core);

  std::ostream& operator<<(std::ostream& out,
                           const GluingTable& table);

  std::ostream& operator<<(std::ostream& out,
                           const CantorAbbreviatedNormalForm& canf);

  std::ostream& operator<<(std::ostream& out,
                           const CantorPerfectNormalForm& cpnf);

  std::ostream& operator<<(std::ostream& out,
                           const ReversePolishNotation& expr);
}

#endif //MAIN_CPP_OUTPUT_HPP
