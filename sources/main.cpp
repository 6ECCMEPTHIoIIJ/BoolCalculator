#include <iostream>

#include "output.hpp"
#include "reverse_polish_notation.hpp"

int main() {
  std::string input_expr;
  std::getline(std::cin, input_expr);
  auto out_expr = BoolCalc::ReversePolishNotation(input_expr);
  auto cnf = BoolCalc::CantorNormalForm(out_expr);
  std::cout << " ~~~~~~~~~~~~~~~~\n"
               "|INPUT EXPRESSION|\n"
               " ~~~~~~~~~~~~~~~~\n" << out_expr << '\n'
            << "\n"
               " ~~~~~~~~~~~\n"
               "|TRUTH TABLE|\n"
               " ~~~~~~~~~~~\n" << cnf.truth_table()
            << "\n"
               " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
               "|CANTOR`S PERFECT NORMAL FORM|\n"
               " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << cnf.perfect() << '\n'
            << "\n"
               " ~~~~~~~~~~~~\n"
               "|GLUING TABLE|\n"
               " ~~~~~~~~~~~~\n" << cnf.gluing_table()
            << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
               "|CANTOR`S ABBREVIATED NORMAL FORM|\n"
               " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << cnf.abbreviated() << '\n'
            << "\n"
               " ~~~~~~~~~~~~~~~~~~~~~~\n"
               "|IMPLICANT QUINE MATRIX|\n"
               " ~~~~~~~~~~~~~~~~~~~~~~\n" << cnf.quine_matrix()
            << "\n"
               " ~~~~~~~~~~\n"
               "|QUINE CORE|\n"
               " ~~~~~~~~~~\n" << cnf.quine_core() << '\n';

  std::cin.get();
  return 0;
}