#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <functional>
#include <exception>
#include <stack>
#include <iomanip>

struct Constituent {
  std::string value;
  bool is_in_use = false;
};

enum class Operations {
  OpenBracket = '(',
  CloseBracket = ')',
  Not = '!',
  And = '&',
  Or = '|',
  Xor = '^',
  Diff = '-',
  Equal = '=',
};

struct Operation {
  Operations type;
  uint8_t priority;
  uint8_t operands_amount;
};

Operation getOperation(const char symbol) {
  switch (symbol) {
    case '[':
    case '{':
    case '(':
      return (Operation) {Operations::OpenBracket, 4, 0};
    case ']':
    case '}':
    case ')':
      return (Operation) {Operations::CloseBracket, 4, 0};
    case '~':
    case '!':
      return (Operation) {Operations::Not, 1, 1};
    case '&':
    case '*':
      return (Operation) {Operations::And, 2, 2};
    case '|':
    case '+':
      return (Operation) {Operations::Or, 3, 2};
    case '^':
      return (Operation) {Operations::Xor, 3, 2};
    case '-':
      return (Operation) {Operations::Diff, 3, 2};
    case '=':
      return (Operation) {Operations::Equal, 3, 2};
    default:
      std::string message = "invalid_operation ";
      message += symbol;
      throw std::runtime_error(message);
  }
}

struct Expression {
  std::string data;
  uint8_t parameters_amount;
};

Expression getExpression() {
  char ch;
  std::string out_expression;
  std::stack<Operation> operations;
  std::array<bool, 26> set_letters{false};
  while (std::cin.get(ch) && ch != '\n') {
    if (!std::isspace(ch)) {
      if (std::isalpha(ch)) {
        set_letters[ch - 'A'] = true;
        out_expression += ch;
      } else {
        Operation operation = getOperation(ch);
        if (operation.operands_amount == 1 ||
            operation.type == Operations::OpenBracket) {
          operations.push(operation);
        } else if (operation.type == Operations::CloseBracket) {
          while (operations.top().type != Operations::OpenBracket) {
            out_expression += static_cast<char>(operations.top().type);
            operations.pop();
          }
          operations.pop();
        } else {
          while (!operations.empty() && operations.top().priority <= operation.priority) {
            out_expression += static_cast<char>(operations.top().type);
            operations.pop();
          }
          operations.push(operation);
        }
      }
    }
  }

  while (!operations.empty()) {
    out_expression += static_cast<char>(operations.top().type);
    operations.pop();
  }

  uint8_t sets_amount = 0;
  for (auto& set: set_letters) {
    sets_amount += set;
  }

  return {out_expression, sets_amount};
}

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

void calculateOperation(std::stack<bool>& operands,
                        const Operations operation_type) {
  switch (operation_type) {
    bool op_1, op_2;
    case Operations::Not:
      op_1 = operands.top();
      operands.pop();
      operands.push(!op_1);
      break;
    case Operations::And:
      op_1 = operands.top();
      operands.pop();
      op_2 = operands.top();
      operands.pop();
      operands.push(op_1 && op_2);
      break;
    case Operations::Or:
      op_1 = operands.top();
      operands.pop();
      op_2 = operands.top();
      operands.pop();
      operands.push(op_1 || op_2);
      break;
    case Operations::Xor:
      op_1 = operands.top();
      operands.pop();
      op_2 = operands.top();
      operands.pop();
      operands.push(xor_(op_1, op_2));
      break;
    case Operations::Diff:
      op_1 = operands.top();
      operands.pop();
      op_2 = operands.top();
      operands.pop();
      operands.push(diff(op_2, op_1));
      break;
    case Operations::Equal:
      op_1 = operands.top();
      operands.pop();
      op_2 = operands.top();
      operands.pop();
      operands.push(op_1 == op_2);
      break;
    default:
      break;
  }
}

bool calculateExpression(Expression& expression,
                         const std::vector<bool>& x) {
  std::stack<bool> operands;
  for (char i: expression.data) {
    if (std::isalpha(i)) {
      operands.push(x[i - 'A']);
    } else {
      auto operation_type = static_cast<Operations>(i);
      calculateOperation(operands, operation_type);
    }
  }

  return operands.top();
}

std::vector<Constituent> getCantorPerfectNormalForm(Expression& expression) {
  std::vector<Constituent> perfect_form{};
  std::cout << "Truth table:\n";
  std::string cur_set_name(3, ' ');
  for (uint8_t i = 0; i < expression.parameters_amount; i++) {
    cur_set_name[1] = static_cast<char>('A' + i);
    std::cout << cur_set_name << "|";
  }
  std::cout << " CPNF\n";
  for (size_t value = 0; value < 1 << expression.parameters_amount; value++) {
    std::vector<bool> bool_vector(expression.parameters_amount);
    for (uint8_t i = 0; i < expression.parameters_amount; i++) {
      bool_vector[i] = static_cast<bool>(value >> (expression.parameters_amount - 1 - i) & 1);
    }
    bool expr_result = calculateExpression(expression, bool_vector);
    std::string x(expression.parameters_amount, '0');
    for (uint8_t i = 0; i < expression.parameters_amount; i++) {
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
                        const Constituent& constituent_2,
                        const uint8_t kParametersAmount) {
  int16_t pos_of_diff = -1;
  for (uint8_t cur_pos = 0; cur_pos < kParametersAmount; cur_pos++) {
    if (constituent_1.value[cur_pos] != constituent_2.value[cur_pos]) {
      pos_of_diff = (pos_of_diff == -1) ? cur_pos : kParametersAmount;
    }
  }

  return pos_of_diff;
}

std::vector<Constituent> glueConstituents(std::vector<Constituent>& perfect_form,
                                          const uint8_t kParametersAmount) {
  std::vector<Constituent> glued{};
  if (!perfect_form.empty()) {
    for (size_t i = 0; i < perfect_form.size() - 1; i++) {
      for (size_t j = i + 1; j < perfect_form.size(); j++) {
        int16_t pos_of_diff = constituentComp(perfect_form[i], perfect_form[j], kParametersAmount);
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
  }

  return glued;
}

void getCantorShortNormalForm(std::vector<Constituent>& constituents,
                              std::vector<Constituent>& short_form,
                              const size_t iter_counter,
                              const uint8_t kParametersAmount) {
  auto glued = glueConstituents(constituents, kParametersAmount);

  std::cout << "\n#" << iter_counter << " iteration of glue:\n" << constituents;
  if (!glued.empty()) {
    getCantorShortNormalForm(glued, short_form, iter_counter + 1, kParametersAmount);
  }
  for (auto& constituent: constituents) {
    if (!constituent.is_in_use) {
      short_form.push_back(constituent);
    }
  }
}

std::vector<Constituent> getCantorShortNormalForm(std::vector<Constituent>& perfect_form,
                                                  const uint8_t kParametersAmount) {
  std::vector<Constituent> short_form{};
  auto glued = glueConstituents(perfect_form, kParametersAmount);

  std::cout << "\n#" << 0 << " iteration of glue:\n" << perfect_form;
  if (!glued.empty()) {
    getCantorShortNormalForm(glued, short_form, 1, kParametersAmount);
  }
  for (auto& constituent: perfect_form) {
    if (!constituent.is_in_use) {
      short_form.push_back(constituent);
    }
  }

  return short_form;
}

bool isOverlap(const Constituent& implicant,
               const Constituent& constituent,
               const uint8_t kParametersAmount) {
  for (size_t i = 0; i < kParametersAmount; i++) {
    if (implicant.value[i] != '-' && implicant.value[i] != constituent.value[i]) {
      return false;
    }
  }

  return true;
}

std::vector<std::vector<bool>> getQuineMatrix(const std::vector<Constituent>& perfect_form,
                                              const std::vector<Constituent>& short_form,
                                              const uint8_t kParametersAmount) {
  std::vector<std::vector<bool>> quine_matrix(short_form.size());
  for (auto& row: quine_matrix) {
    row = std::vector<bool>(perfect_form.size());
  }

  for (size_t j = 0; j < short_form.size(); j++) {
    for (size_t i = 0; i < perfect_form.size(); i++) {
      if (isOverlap(short_form[j], perfect_form[i], kParametersAmount)) {
        quine_matrix[j][i] = true;
      }
    }
  }

  return quine_matrix;
}

void outQuineMatrix(const std::vector<Constituent>& perfect_form,
                    const std::vector<Constituent>& short_form,
                    const std::vector<std::vector<bool>>& quine_matrix,
                    const uint8_t kParametersAmount) {
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
  if (!quine_matrix.empty()) {
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

void outForm(const std::vector<Constituent>& form,
             const uint8_t kParametersAmount) {
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
  const size_t kAggregatorWidth = 30;
  const size_t kLineLength = 50;
  Expression expression;
  std::vector<Constituent> cantor_perfect_normal_form, cantor_short_normal_form;
  std::vector<std::vector<bool>> quine_matrix;
  std::vector<std::vector<Constituent>> dead_end_cantor_normal_forms;
  while (true) {
    std::cout << "Input your expression:\n"
              << std::setfill('-') << std::setw(kLineLength) << "\n"
              << std::left << std::setfill(' ')
              << std::setw(kAggregatorWidth + 8) << "valid names for sets are:" << "[A..Z]\n"
              << "valid operations are:\n"
              << std::right << std::setfill(' ')
              << std::setw(kAggregatorWidth) << "adding to universe" << '|' << std::setw(7) << "(not): " << "! ~\n"
              << std::setw(kAggregatorWidth) << "intersection" << '|' << std::setw(7) << "(and): " << "& *\n"
              << std::setw(kAggregatorWidth) << "union" << '|' << std::setw(7) << "(or): " << "| +\n"
              << std::setw(kAggregatorWidth) << "difference" << '|' << std::setw(7) << ": " << "-\n"
              << std::setw(kAggregatorWidth) << "symmetric difference" << '|' << std::setw(7) << "(xor): " << "^\n"
              << std::setw(kAggregatorWidth) << "equality" << '|' << std::setw(7) << ": " << "=\n"
              << std::setw(kAggregatorWidth) << "brackets" << '|' << std::setw(7) << ": " << "( { [ ) } ]\n"
              << std::setfill('-') << std::setw(kLineLength) << "\n";
    expression = getExpression();
    std::cout << expression.data << '\n';
    cantor_perfect_normal_form = getCantorPerfectNormalForm(expression);
    std::cout << "\nCantor`s perfect normal form:\n" << cantor_perfect_normal_form;
    outForm(cantor_perfect_normal_form,
            expression.parameters_amount);
    std::cout << "\nGluing constituents:\n";
    cantor_short_normal_form = getCantorShortNormalForm(cantor_perfect_normal_form,
                                                             expression.parameters_amount);
    std::cout << "\nCantor`s short normal form:\n" << cantor_short_normal_form;
    outForm(cantor_short_normal_form,
            expression.parameters_amount);
    quine_matrix = getQuineMatrix(cantor_perfect_normal_form, cantor_short_normal_form,
                                       expression.parameters_amount);
    std::cout << "\nQuine matrix:\n";
    outQuineMatrix(cantor_perfect_normal_form, cantor_short_normal_form, quine_matrix,
                   expression.parameters_amount);
    dead_end_cantor_normal_forms = getDeadEndCantorNormalForms(cantor_short_normal_form,
                                                                    getDeadEnds(quine_matrix));
    std::cout << "\nDead end Cantor`s normal forms:\n";
    size_t i = 0;
    for (auto& dead_end_cantor_normal_form: dead_end_cantor_normal_forms) {
      std::cout << "\n#" << i++ << " form:\n" << dead_end_cantor_normal_form;
      outForm(dead_end_cantor_normal_form,
              expression.parameters_amount);
    }

    std::cout << "\nDo you want to continue? (Y/N)...\n";
    char exit_ch;
    std::cin >> exit_ch;
    if (exit_ch == 'N') {
      return 0;
    }
  }
}

