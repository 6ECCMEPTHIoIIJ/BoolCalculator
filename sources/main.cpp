#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <functional>

constexpr uint8_t kParametersAmount = 4;

struct Constituent {
  std::string value;
  bool is_in_use = false;
};

std::ostream& operator<<(std::ostream& out,
                         std::vector<Constituent>& v) {
  for (auto& i: v) {
    std::cout << i.value << (i.is_in_use ? "*" : "") << '\n';
  }

  return out;
}

bool diff(const bool a,
          const bool b) {
  return a & !b;
}

bool xor_(const bool a,
          const bool b) {
  return diff(a, b) || diff(b, a);
}

std::vector<Constituent> getCantorPerfectNormalForm(const std::function<bool(std::array<bool, kParametersAmount>)>& calculateExpr) {
  std::vector<Constituent> perfect_form{};
  std::cout << "Truth table:\n";
  std::string cur_set_name(3, ' ');
  for (uint8_t i = 0; i < kParametersAmount; i++) {
    cur_set_name[1] = static_cast<char>('A' + i);
    std::cout << cur_set_name << "|";
  }
  std::cout << " CPNF\n";
  for (size_t value = 0; value < 1 << kParametersAmount; value++) {
    std::array<bool, kParametersAmount> bool_vector{};
    for (uint8_t i = 0; i < kParametersAmount; i++) {
      bool_vector[i] = static_cast<bool>(value >> (kParametersAmount - 1 - i) & 1);
    }
    bool expr_result = calculateExpr(bool_vector);
    std::string x(kParametersAmount, '0');
    for (uint8_t i = 0; i < kParametersAmount; i++) {
      x[i] = static_cast<char>(x[i] + bool_vector[i]);
      std::cout << (bool_vector[i] ? " U " : " - ") << '|';
    }
    std::cout << (expr_result ? " Yes\n" : " No\n");
    if (expr_result) {
      perfect_form.push_back((Constituent) {x});
    }
  }

  return perfect_form;
}

int16_t constituentComp(const Constituent& constituent_1,
                        const Constituent& constituent_2) {
  int16_t pos_of_diff = -1;
  for (uint8_t cur_pos = 0; cur_pos < kParametersAmount; cur_pos++) {
    if (constituent_1.value[cur_pos] != constituent_2.value[cur_pos]) {
      pos_of_diff = (pos_of_diff == -1) ? cur_pos : kParametersAmount;
    }
  }

  return pos_of_diff;
}

std::vector<Constituent> glueConstituents(std::vector<Constituent>& perfect_form) {
  std::vector<Constituent> glued{};
  for (size_t i = 0; i < perfect_form.size() - 1; i++) {
    for (size_t j = i + 1; j < perfect_form.size(); j++) {
      int16_t pos_of_diff = constituentComp(perfect_form[i], perfect_form[j]);
      if (pos_of_diff == -1) {
        perfect_form.erase(perfect_form.begin() + j);
        j--;
      } else if (pos_of_diff != kParametersAmount) {
        perfect_form[i].is_in_use = true;
        perfect_form[j].is_in_use = true;
        std::string glued_vector = perfect_form[i].value;
        glued_vector[pos_of_diff] = '-';

        glued.push_back((Constituent) {glued_vector});
      }
    }
  }

  return glued;
}

void getCantorShortNormalForm(std::vector<Constituent>& constituents,
                              std::vector<Constituent>& short_form,
                              const size_t iter_counter) {
  auto glued = glueConstituents(constituents);

  std::cout << "\n#" << iter_counter << " iteration of glue:\n" << constituents;
  if (!glued.empty()) {
    getCantorShortNormalForm(glued, short_form, iter_counter + 1);
  }
  for (auto& constituent: constituents) {
    if (!constituent.is_in_use) {
      short_form.push_back(constituent);
    }
  }
}

std::vector<Constituent> getCantorShortNormalForm(std::vector<Constituent>& perfect_form) {
  std::vector<Constituent> short_form{};
  auto glued = glueConstituents(perfect_form);

  std::cout << "\n#" << 0 << " iteration of glue:\n" << perfect_form;
  if (!glued.empty()) {
    getCantorShortNormalForm(glued, short_form, 1);
  }
  for (auto& constituent: perfect_form) {
    if (!constituent.is_in_use) {
      short_form.push_back(constituent);
    }
  }

  return short_form;
}

bool isOverlap(const Constituent& implicant,
               const Constituent& constituent) {
  for (size_t i = 0; i < kParametersAmount; i++) {
    if (implicant.value[i] != '-' && implicant.value[i] != constituent.value[i]) {
      return false;
    }
  }

  return true;
}

std::vector<std::vector<bool>> getQuineMatrix(const std::vector<Constituent>& perfect_form,
                                              const std::vector<Constituent>& short_form) {
  std::vector<std::vector<bool>> quine_matrix(short_form.size());
  for (auto& row: quine_matrix) {
    row = std::vector<bool>(perfect_form.size());
  }

  for (size_t j = 0; j < short_form.size(); j++) {
    for (size_t i = 0; i < perfect_form.size(); i++) {
      if (isOverlap(short_form[j], perfect_form[i])) {
        quine_matrix[j][i] = true;
      }
    }
  }

  return quine_matrix;
}

void outQuineMatrix(const std::vector<Constituent>& perfect_form,
                    const std::vector<Constituent>& short_form,
                    const std::vector<std::vector<bool>>& quine_matrix) {
  std::string aggregate(kParametersAmount, '_');
  std::cout << aggregate << '|';
  for (auto& constituent: perfect_form) {
    std::cout << constituent.value << '|';
  }
  aggregate = std::string(kParametersAmount, ' ');
  std::cout << '\n';
  for (size_t j = 0; j < short_form.size(); j++) {
    std::cout << short_form[j].value << '|';
    for (auto i: quine_matrix[j]) {
      aggregate[aggregate.size() / 2] = i ? '+' : ' ';
      std::cout << aggregate << '|';
    }
    std::cout << '\n';
  }
}

bool all(const std::vector<bool>& v,
         const std::function<bool(bool)>& condition) {
  for (auto i: v) {
    if (!condition(i)) {
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

std::vector<size_t> getDeadEnds(const std::vector<std::vector<bool>>& quine_matrix) {
  std::vector<size_t> dead_ends{};
  for (size_t value = 0; value < 1 << quine_matrix.size(); value++) {
    std::vector<bool> coating(quine_matrix.data()->size(), false);
    for (size_t j = 0; j < quine_matrix.size(); j++) {
      if (value >> j & 1) {
        for (size_t i = 0; i < quine_matrix.data()->size(); i++) {
          if (quine_matrix[j][i]) {
            coating[i] = true;
          }
        }
      }
    }
    if (all(coating, [](const bool value) { return value; }) && needToInclude(dead_ends, value)) {
      dead_ends.push_back(value);
    }
  }

  return dead_ends;
}

std::vector<std::vector<Constituent>> getDeadEndCantorNormalForms(const std::vector<Constituent>& short_form,
                                                                  const std::vector<size_t>& dead_ends) {
  std::vector<std::vector<Constituent>> dead_end_forms(dead_ends.size());
  for (size_t j = 0; j < dead_ends.size(); j++) {
    for (size_t i = 0; i < short_form.size(); i++) {
      if (dead_ends[j] >> i & 1) {
        dead_end_forms[j].push_back(short_form[i]);
      }
    }
  }

  return dead_end_forms;
}

void outForm(const std::vector<Constituent>& form) {
  bool is_first_set = true;
  for (auto& constituent: form) {
    std::cout << (is_first_set ? "" : " U ");
    for (uint8_t i = 0; i < kParametersAmount; i++) {
      switch (constituent.value[i]) {
        case '1':
          std::cout << static_cast<char>('A' + i);
          break;
        case '0':
          std::cout << '!' << static_cast<char>('A' + i);
          break;
        default:
          break;
      }
    }
    is_first_set = false;
  }
  std::cout << '\n';
}

int main() {
  std::setvbuf(stdout, nullptr, _IOFBF, 512);
  std::ios_base::sync_with_stdio(false);
//  4
//  auto cantor_perfect_normal_form = getCantorPerfectNormalForm([](std::array<bool, kParametersAmount> x) {
//    return xor_(diff(x[0] && x[2], x[1] && diff(x[3], x[2])), diff(x[3], x[0]));
//  });
//  6
//  auto cantor_perfect_normal_form = getCantorPerfectNormalForm([](std::array<bool, kParametersAmount> x) {
//    return diff(diff(xor_(x[2], diff(x[3], x[0])) || x[1],  x[2] && diff(x[1], x[0])), x[3]);
//  });
//  7
  auto cantor_perfect_normal_form = getCantorPerfectNormalForm([](std::array<bool, kParametersAmount> x) {
    return xor_(diff(diff(x[0], xor_(x[2], x[0])) || x[1], x[3]), diff(x[2], x[1]));
  });
//  10
//  auto cantor_perfect_normal_form = getCantorPerfectNormalForm([](std::array<bool, kParametersAmount> x) {
//    return xor_(xor_(diff(x[2] || x[1], x[3]), diff(x[2], x[1])), x[0]) && x[0];
//  });
//  15
//  auto cantor_perfect_normal_form = getCantorPerfectNormalForm([](std::array<bool, kParametersAmount> x) {
//    return !xor_(xor_(diff(x[0], xor_(x[1], x[2])) || x[3], diff(x[1], x[0])), x[2]);
//  });
//  18
//  auto cantor_perfect_normal_form = getCantorPerfectNormalForm([](std::array<bool, kParametersAmount> x) {
//    return diff(xor_(x[1], diff(x[0], x[2])) || x[3], x[0] && diff(x[1], x[2]));
//  });
  std::cout << "\nCantor`s perfect normal form:\n" << cantor_perfect_normal_form;
  outForm(cantor_perfect_normal_form);
  std::cout << "\nGluing constituents:\n";
  auto cantor_short_normal_form = getCantorShortNormalForm(cantor_perfect_normal_form);
  std::cout << "\nCantor`s short normal form:\n" << cantor_short_normal_form;
  outForm(cantor_short_normal_form);
  auto quine_matrix = getQuineMatrix(cantor_perfect_normal_form, cantor_short_normal_form);
  std::cout << "\nQuine matrix:\n";
  outQuineMatrix(cantor_perfect_normal_form, cantor_short_normal_form, quine_matrix);
  auto dead_end_cantor_normal_forms = getDeadEndCantorNormalForms(cantor_short_normal_form, getDeadEnds(quine_matrix));
  std::cout << "\nDead end Cantor`s normal forms:\n";
  size_t i = 0;
  for (auto& dead_end_cantor_normal_form: dead_end_cantor_normal_forms) {
    std::cout << "\n#" << i++ << " form:\n" << dead_end_cantor_normal_form;
    outForm(dead_end_cantor_normal_form);
  }

  return 0;
}

