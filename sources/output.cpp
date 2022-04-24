
#include "output.hpp"
#include <string>

namespace BoolCalc {

  namespace Output {

    Mode out_mode;

    void cantor() {
      out_mode = Mode::CANTOR;
    }

    void boolean() {
      out_mode = Mode::BOOLEAN;
    }

    Mode getMode() {
      return out_mode;
    }
  }

  std::ostream& operator<<(std::ostream& out,
                           const GluingTable& table) {
    Output::boolean();

    out << std::setfill('_');
    for (uint8_t i = 0; i < table.params_amount() + 1; i++) {
      out << std::setw(table.params_amount() + 2)
          << static_cast<char>('0' + i) << '_' << (i != table.params_amount() ? "|" : "");
    }
    out << '\n';

    for (const auto& row: table) {
      size_t max_h = row.data()->size();
      for (const auto& column: row) {
        if (column.size() > max_h) {
          max_h = column.size();
        }
      }

      out << std::setfill(' ');
      for (size_t h = 0; h < max_h; h++) {
        for (auto column = row.begin(); column != row.end(); column++) {
          if (h < column->size()) {
            out << std::setw(2) << (*column)[h] << ((*column)[h].is_in_use() ? '*' : ' ') << ' ';
          } else {
            out << std::setw(table.params_amount() + 3) << ' ';
          }
          out << (column != row.end() - 1 ? "|" : "");
        }
        out << '\n';
      }
      out << '\n';
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out,
                           const Constituent& c) {
    const uint32_t kUniverse = (1 << (c.size() * 2)) - (1 << c.size());

    if (Output::getMode() == Output::Mode::CANTOR && c.isEqualTo(Constituent(kUniverse, c.size()))) {
      out << 'U';
    } else {
      for (uint8_t i = c.size(); i != 0; i--) {
        const bool skipped_bit = c.value() >> (c.size() + i - 1) & 1;
        const bool significant_bit = c.value() >> (i - 1) & 1;
        switch (Output::getMode()) {
          case Output::Mode::CANTOR :
            if (!skipped_bit) {
              out << (significant_bit ? "" : "!") << static_cast<char>('A' + c.size() - i);
            }
            break;
          case Output::Mode::BOOLEAN :
            out << static_cast<char>(skipped_bit ? '-' : significant_bit + '0');
        }
      }
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out,
                           const TruthTable& table) {
    for (uint8_t i = 0; i < table.params_amount(); i++) {
      out << '_' << static_cast<char>('A' + i) << '_' << '|';
    }

    for (size_t i = 0; i < table.operations_amount(); i++) {
      out << std::setfill('_') << std::setw(2) << i + 1 << '_' << '|';
    }

    out << std::setfill('_') << std::setw(8) << "RESULT" << '_' << '\n';

    size_t bool_vector = 0;
    for (auto& row: table) {
      const bool expr_result = row.back();
      for (uint8_t i = table.params_amount(); i != 0; i--) {
        out << ' ' << (bool_vector >> (i - 1) & 1 ? 'U' : '-') << ' ' << '|';
      }
      for (const auto& opertaion_result: table.at(bool_vector)) {
        out << ' ' << (opertaion_result ? 'U' : '-') << ' ' << '|';
      }
      if (expr_result) {
        out << ' ' << "YES: [" << bool_vector << ']';
      } else {
        out << ' ' << "NO";
      }
      out << '\n';

      bool_vector++;
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out,
                           const CantorPerfectNormalForm& cpnf) {
    Output::cantor();
    if (cpnf.empty()) {
      out << "O";
    } else {
      for (size_t i = 0; i < cpnf.size() - 1; i++) {
        out << cpnf[i] << " U ";
      }
      out << cpnf[cpnf.size() - 1];
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out,
                           const QuineMatrix& matrix) {
    Output::boolean();

    out << std::string(matrix.params_amount() + 2, '_') << '|';
    for (size_t w = 0; w < matrix.width(); w++) {
      out << '_' << matrix.constituents()[w] << '_' << (w != matrix.width() - 1 ? '|' : '\n');
    }

    std::string tmp(matrix.params_amount() + 2, ' ');
    for (size_t h = 0; h < matrix.height(); h++) {
      out << ' ' << matrix.implicants()[h] << ' ' << '|';
      for (size_t w = 0; w < matrix.width(); w++) {
        tmp[(matrix.params_amount() + 2) / 2] = matrix.overlap_table()[h][w] ? '+' : ' ';
        out << tmp << (w != matrix.width() - 1 ? '|' : '\n');
      }
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out,
                           const CantorAbbreviatedNormalForm& canf) {
    Output::cantor();

    if (canf.empty()) {
      out << "O";
    } else {
      for (size_t i = 0; i < canf.size() - 1; i++) {
        out << canf[i] << " U ";
      }
      out << canf[canf.size() - 1];
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out,
                           const QuineCore& core) {
    Output::cantor();
    const std::vector<bool> kEmptyCore(core.size(), false);

    if (core.core() == kEmptyCore) {
      out << "O";
    } else {
      for (size_t i = 0; i < core.size() - 1; i++) {
        if (core.core()[i]) {
          out << core.implicants()[i] << " U ";
        }
      }
      out << core.implicants()[core.size() - 1];
    }

    return out;
  }

  std::ostream& operator<<(std::ostream& out,
                           const ReversePolishNotation& expr) {
    for (const auto& expr_member: expr.input_expression()) {
      if (std::holds_alternative<Operand>(expr_member)) {
        out << static_cast<char>(std::get<Operand>(expr_member).index() + 'A');
      } else {
        Operation operation = std::get<Operation>(expr_member);
        out << operation.presenting_symbol();
        if (operation.type() != Operation::Type::CloseBracket &&
            operation.type() != Operation::Type::OpenBracket) {
          out << '{' << operation.index() + 1 << '}';
        }
      }
    }

    return out;
  }
}