#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>

constexpr uint8_t kParametersAmount = 5;

struct Implicant {
  std::string value;
  bool is_in_use = false;
};

struct QuineMatrix {
  std::vector<Implicant> cantor_perfect_normal_form;
  std::vector<Implicant> not_used_implicants;
  std::vector<std::vector<bool>> data;
};

bool diff(const bool x_1,
          const bool x_2) {
  return x_1 && !x_2;
}

bool xor_(const bool x_1,
          const bool x_2) {
  return diff(x_1, x_2) ||
         diff(x_2, x_1);
}

std::ostream& operator<<(std::ostream& out,
                         Implicant& imp) {
  std::cout << imp.value << (imp.is_in_use ? "*" : " \b");
  return out;
}

std::ostream& operator<<(std::ostream& out,
                         std::vector<Implicant>& v) {
  for (auto& i: v) {
    std::cout << i << '\n';
  }

  return out;
}

std::ostream& operator<<(std::ostream& out,
                         std::vector<size_t>& v) {
  for (auto& i: v) {
    std::cout << i << '\n';
  }

  return out;
}

std::ostream& operator<<(std::ostream& out,
                         QuineMatrix& m) {
  std::string cur_cell(kParametersAmount, '_');
  std::cout << cur_cell << '|';
  for (auto& v: m.cantor_perfect_normal_form) {
    std::cout << v.value << '|';
  }
  std::cout << '\n';
  for (size_t h = 0; h < m.data.size(); h++) {
    std::cout << m.not_used_implicants[h] << '|';
    for (auto i: m.data[h]) {
      cur_cell = std::string(kParametersAmount, ' ');
      if (i) {
        cur_cell[cur_cell.size() / 2] = '+';
      }
      std::cout << cur_cell << '|';
    }
    std::cout << '\n';
  }

  return out;
}

std::vector<Implicant> getCantorPerfectNormalForm(const std::function<bool(std::string)>& expr) {
  std::vector<Implicant> cantor_perfect_normal_form{};
  std::cout << "Truth table\n";
  for (uint8_t i = 0; i < kParametersAmount; i++) {
    std::cout << "x_" << static_cast<size_t>(i) << '|';
  }
  std::cout << std::setw(2) << "F" << '\n';
  for (size_t value = 0; value < 1 << kParametersAmount; value++) {
    std::string x{};
    for (uint8_t i = kParametersAmount; i != 0; i--) {
      x += static_cast<char>(value >> (i - 1) & 1);
    }
    bool result = expr(x);
    for (auto& ch: x) {
      ch += '0';
      std::string cur_value(3, ' ');
      cur_value[1] = ch == '1' ? 'U' : '-';
      std::cout << cur_value << '|';
    }
    std::cout << std::setw(2) << (result ? 'U' : '-') << '\n';
    if (result) {
      cantor_perfect_normal_form.push_back({x});
    }
  }

  return cantor_perfect_normal_form;
}

int8_t getPosOfDiff(const std::string& line_1,
                    const std::string& line_2) {
  int8_t pos_of_diff = -1;
  for (int8_t i = 0; i < kParametersAmount; i++) {
    if (line_1[i] != line_2[i]) {
      pos_of_diff = static_cast<int8_t>((pos_of_diff == -1) ? i : kParametersAmount);
    }
  }

  return pos_of_diff;
}

void glueImplicants(std::vector<Implicant>& implicants,
                    std::vector<Implicant>& not_used_implicants,
                    const size_t n) {
  std::vector<Implicant> glued_implicants{};

  for (size_t implicant_1_i = 0;
       implicant_1_i < implicants.size();
       implicant_1_i++) {
    for (size_t implicant_2_i = implicant_1_i + 1;
         implicant_2_i < implicants.size();
         implicant_2_i++) {
      int8_t pos_of_diff = getPosOfDiff(implicants[implicant_1_i].value, implicants[implicant_2_i].value);
      if (pos_of_diff == -1) {
        implicants.erase(implicants.begin() + implicant_2_i);
        implicant_2_i--;
      } else if (pos_of_diff != kParametersAmount) {
        implicants[implicant_1_i].is_in_use = true;
        implicants[implicant_2_i].is_in_use = true;
        std::string x = implicants[implicant_1_i].value;
        x[pos_of_diff] = '-';

        glued_implicants.push_back({x});
      }
    }
  }

  std::cout << "\n#" << n << " iter of glue\n" << implicants;
  if (!glued_implicants.empty()) {
    glueImplicants(glued_implicants, not_used_implicants, n + 1);
  }
  for (auto& cur_implicant: implicants) {
    if (!cur_implicant.is_in_use) {
      not_used_implicants.push_back(cur_implicant);
    }
  }
}

std::vector<Implicant> glueImplicants(std::vector<Implicant>& implicants) {
  std::vector<Implicant> not_used_implicants{};
  std::vector<Implicant> glued_implicants{};

  for (size_t implicant_1_i = 0;
       implicant_1_i < implicants.size();
       implicant_1_i++) {
    for (size_t implicant_2_i = implicant_1_i + 1;
         implicant_2_i < implicants.size();
         implicant_2_i++) {
      int8_t pos_of_diff = getPosOfDiff(implicants[implicant_1_i].value, implicants[implicant_2_i].value);
      if (pos_of_diff == -1) {
        implicants.erase(implicants.begin() + implicant_2_i);
        implicant_2_i--;
      } else if (pos_of_diff != kParametersAmount) {
        implicants[implicant_1_i].is_in_use = true;
        implicants[implicant_2_i].is_in_use = true;
        std::string x = implicants[implicant_1_i].value;
        x[pos_of_diff] = '-';

        glued_implicants.push_back({x});
      }
    }
  }

  std::cout << "\n#" << 0 << " iter of glue\n" << implicants;
  if (!glued_implicants.empty()) {
    glueImplicants(glued_implicants, not_used_implicants, 1);
  }
  for (auto& cur_implicant: implicants) {
    if (!cur_implicant.is_in_use) {
      not_used_implicants.push_back(cur_implicant);
    }
  }

  return not_used_implicants;
}

bool isOverlap(const std::string& line_1,
               const std::string& line_2) {
  for (size_t i = 0; i < kParametersAmount; i++) {
    if (line_1[i] != '-' && line_1[i] != line_2[i]) {
      return false;
    }
  }

  return true;
}

QuineMatrix getQuineMatrix(
    const std::vector<Implicant>& cantor_perfect_normal_form,
    const std::vector<Implicant>& not_used_implicants) {
  std::vector<std::vector<bool>> quine_matrix(
      not_used_implicants.size());
  for (auto& row: quine_matrix) {
    row = std::vector<bool>(cantor_perfect_normal_form.size());
  }

  for (size_t row_i = 0; row_i < quine_matrix.size(); row_i++) {
    for (size_t col_i = 0; col_i < quine_matrix.data()->size(); col_i++) {
      if (isOverlap(not_used_implicants[row_i].value,
                    cantor_perfect_normal_form[col_i].value)) {
        quine_matrix[row_i][col_i] = true;
      }
    }
  }

  return {cantor_perfect_normal_form, not_used_implicants, quine_matrix};
}

bool allTrue(const std::vector<bool>& v) {
  for (auto i: v) {
    if (!i) {
      return false;
    }
  }

  return true;
}

bool needToInclude(std::vector<size_t>& v,
                   const size_t value) {
  for (auto vector_value: v) {
    if ((value & vector_value) == vector_value) {
      return false;
    }
  }

  return true;
}

std::vector<size_t> getDeadEndCantorForms(const QuineMatrix& quine_matrix) {
  std::vector<size_t> dead_end_cantor_forms{};

  for (size_t value = 0; value < 1 << quine_matrix.not_used_implicants.size(); value++) {
    std::vector<bool> coating(quine_matrix.cantor_perfect_normal_form.size(), false);
    for (size_t row_i = 0; row_i < quine_matrix.not_used_implicants.size(); row_i++) {
      if (value >> row_i & 1) {
        for (size_t col_i = 0; col_i < quine_matrix.cantor_perfect_normal_form.size(); col_i++) {
          if (quine_matrix.data[row_i][col_i]) {
            coating[col_i] = true;
          }
        }
      }
    }
    if (allTrue(coating) && needToInclude(dead_end_cantor_forms, value)) {
      dead_end_cantor_forms.push_back(value);
    }
  }

  return dead_end_cantor_forms;
}

void printDeadEndCantorForms(const std::vector<size_t>& dead_end_cantor_forms,
                             const std::vector<Implicant>& not_used_implicants) {
  for (auto dead_end_cantor_form: dead_end_cantor_forms) {
    bool is_first_form = true;
    for (uint8_t i = not_used_implicants.size(); i != 0; i--) {
      if (dead_end_cantor_form >> (i - 1) & 1) {
        std::cout << (is_first_form ? "(" : " U (");
        is_first_form = false;
        bool is_first_set = true;
        for (uint8_t set_i = 0; set_i < kParametersAmount; set_i++) {
          switch (not_used_implicants[i - 1].value[set_i]) {
            case '1':
              std::cout << (is_first_set ? "" : " ") << "x_" << static_cast<size_t>(set_i);
              is_first_set = false;
              break;
            case '0':
              std::cout << (is_first_set ? "" : " ") << "!x_" << static_cast<size_t>(set_i);
              is_first_set = false;
              break;
            default:
              break;
          }
        }
        std::cout << ")";
      }
    }
    std::cout << "\n";
  }
}

int main() {
  std::setvbuf(stdout, nullptr, _IOFBF, 512);
  std::ios_base::sync_with_stdio(false);
  // Вариант 7
  auto cantor_perfect_normal_form = getCantorPerfectNormalForm(
      [](const std::string& x) {
        return xor_(diff(diff(x[0], xor_(x[2], x[0])) || x[1], x[3]), diff(x[2], x[1]));
      });
  // Вариант 10
//  auto cantor_perfect_normal_form = getCantorPerfectNormalForm(
//      [](const std::string& x) {
//        return xor_(xor_(diff(x[2] || x[1], x[3]),
//                         diff(x[2], x[1])),
//                    x[0]) && x[0];
//      });
  // Вариант 18
//  auto cantor_perfect_normal_form = getCantorPerfectNormalForm(
//      [](const std::string& x) {
//        return diff(xor_(x[1], diff(x[0], x[2])) || x[3], x[0] && diff(x[1], x[2]));
//      });

  std::cout << "\nCommon implicatns:\n" << cantor_perfect_normal_form;
  auto not_used_implicants = glueImplicants(cantor_perfect_normal_form);
  QuineMatrix quine_matrix = getQuineMatrix(cantor_perfect_normal_form, not_used_implicants);
  std::cout << "\nQuine matrix\n" << quine_matrix;
  std::vector<size_t> dead_end_cantor_forms = getDeadEndCantorForms(quine_matrix);
  std::cout << "\nDead end Cantor normal forms\n";
  printDeadEndCantorForms(dead_end_cantor_forms, not_used_implicants);
//  std::cin.get();
  return 0;
}