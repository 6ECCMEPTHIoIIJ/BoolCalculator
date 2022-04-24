#include "reverse_polish_notation.hpp"

namespace BoolCalc {

  Operation::Operation(const char presenting_symbol) {
    type_ = operations_dictionary().find(presenting_symbol)->second;
    switch (type_) {
      case Type::OpenBracket:
        presenting_symbol_ = "(";
        break;
      case Type::CloseBracket:
        presenting_symbol_ = ")";
        break;
      case Type::Equality:
        operands_amount_ = 2;
        association_ = Association::Right;
        priority_ = 2;
        function_ = [](const bool a,
                       const bool b) { return a == b; };
        presenting_symbol_ = "==";
        break;
      case Type::Inequality:
        operands_amount_ = 2;
        association_ = Association::Right;
        priority_ = 2;
        function_ = [](const bool a,
                       const bool b) { return a != b; };
        presenting_symbol_ = "!=";
        break;
        //TODO: Sub, StrictSub
      case Type::Not:
        operands_amount_ = 1;
        association_ = Association::Left;
        function_ = [](const bool a,
                       const bool b = false) { return !a; };
        presenting_symbol_ = "!";
        break;
      case Type::And:
        operands_amount_ = 2;
        association_ = Association::Right;
        priority_ = 0;
        function_ = [](const bool a,
                       const bool b) { return a && b; };
        presenting_symbol_ = "&";
        break;
      case Type::Or:
        operands_amount_ = 2;
        association_ = Association::Right;
        priority_ = 1;
        function_ = [](const bool a,
                       const bool b) { return a || b; };
        presenting_symbol_ = "|";
        break;
      case Type::Diff:
        operands_amount_ = 2;
        association_ = Association::Right;
        priority_ = 1;
        function_ = [](const bool a,
                       const bool b) { return a && !b; };
        presenting_symbol_ = "-";
        break;
      case Type::Xor:
        operands_amount_ = 2;
        association_ = Association::Right;
        priority_ = 1;
        function_ = [](const bool a,
                       const bool b) { return a != b; };
        presenting_symbol_ = "^";
        break;
      case Type::Sub:
        association_ = Association::Right;
        priority_ = 2;
        presenting_symbol_ = "<=";
        break;
      case Type::StrictSub:
        association_ = Association::Right;
        priority_ = 2;
        presenting_symbol_ = "<";
        break;
    }
  }

  Operation::Type Operation::type() const {
    return type_;
  }

  Operation::Association Operation::association() const {
    return association_;
  }

  uint8_t Operation::operands_amount() const {
    return operands_amount_;
  }

  uint8_t Operation::priority() const {
    return priority_;
  }

  void Operation::index(const size_t index) {
    index_ = index;
  }

  [[nodiscard]] size_t Operation::index() const {
    return index_;
  }

  const std::string& Operation::presenting_symbol() const {
    return presenting_symbol_;
  }

  bool Operation::operator()(const bool a,
                             const bool b = false) {
    return function_(a, b);
  }

  const std::unordered_map<char, Operation::Type>& operations_dictionary() {
    const static std::unordered_map<char, Operation::Type> dictionary{
        {'(',  Operation::Type::OpenBracket},
        {'[',  Operation::Type::OpenBracket},
        {'{',  Operation::Type::OpenBracket},
        {')',  Operation::Type::CloseBracket},
        {']',  Operation::Type::CloseBracket},
        {'}',  Operation::Type::CloseBracket},
        {'=',  Operation::Type::Equality},
        {'/',  Operation::Type::Inequality},
        {'?',  Operation::Type::Sub},
        {'<',  Operation::Type::StrictSub},
        {'!',  Operation::Type::Not},
        {'~',  Operation::Type::Not},
        {'*',  Operation::Type::And},
        {'&',  Operation::Type::And},
        {'+',  Operation::Type::Or},
        {'|',  Operation::Type::Or},
        {'-',  Operation::Type::Diff},
        {'\\', Operation::Type::Diff},
        {'^',  Operation::Type::Xor},
    };

    return dictionary;
  }

  Operand::Operand(const char presenting_symbol) {
    index_ = presenting_symbol - 'A';
  }

  uint8_t Operand::index() const {
    return index_;
  }

  size_t countExpressionMembers(const std::string& input_expression) {
    size_t counter = 0;
    for (const auto& ch: input_expression) {
      if (std::isspace(ch)) {
        continue;
      }

      counter++;
    }

    return counter;
  }

  void ReversePolishNotation::getLastOperation(std::stack<std::reference_wrapper<Operation>>& operations,
                                               size_t& operation_index) {
    out_expression_.emplace_back(operations.top());
    operations.top().get().index(operation_index++);
    operations.pop();
  }

  ReversePolishNotation::ReversePolishNotation(const std::string& input_expression) {
    std::stack<std::reference_wrapper<Operation>> operations;
    size_t expression_members_amount = countExpressionMembers(input_expression);
    input_expression_.reserve(expression_members_amount); // костыль, но без него работать не будет
    out_expression_.reserve(expression_members_amount);

    size_t operation_index = 0;
    for (const auto& ch: input_expression) {
      if (std::isspace(ch)) {
        continue;
      }

      if (std::isalpha(ch)) {
        Operand operand(ch);
        input_expression_.emplace_back(operand);
        out_expression_.emplace_back(operand);
        params_amount_ = std::max(params_amount_, static_cast<uint8_t>(operand.index() + 1));
      } else {
        const Operation operation(ch);
        input_expression_.emplace_back(operation);
        auto& last_operation = std::get<Operation>(input_expression_.back());
        if (operation.type() == Operation::Type::OpenBracket) { // в params_amount_ может лежать и мусор, поэтому
          // условия объединить нельзя
          operations.push(last_operation);
        } else if (operation.type() == Operation::Type::CloseBracket) {
          while (operations.top().get().type() != Operation::Type::OpenBracket) {
            getLastOperation(operations, operation_index);
          }

          operations.pop();
        } else if (operation.operands_amount() == 1) {
          operations.push(last_operation);
        } else {
          while (!operations.empty() &&
                 operations.top().get().type() != Operation::Type::OpenBracket &&
                 (operation.priority() >= operations.top().get().priority() ||
                  operations.top().get().association() == Operation::Association::Left)) {
            getLastOperation(operations, operation_index);
          }

          operations.push(last_operation);
        }
      }
    }

    while (!operations.empty()) {
      getLastOperation(operations, operation_index);
    }
  }

  uint8_t ReversePolishNotation::params_amount() const {
    return params_amount_;
  }

  bool getValue(const Operand& operand,
                const size_t bool_vector,
                const uint8_t params_amount) {
    return (bool_vector >> (params_amount - operand.index() - 1)) & 1;
  }

  const std::vector<std::variant<Operation, Operand>>& ReversePolishNotation::input_expression() const {
    return input_expression_;
  }

  std::vector<bool> ReversePolishNotation::calculate(const size_t bool_vector) const {
    std::vector<bool> results_of_calculating;
    std::stack<bool> operands;
    for (const auto& expr_member: out_expression_) {
      if (std::holds_alternative<Operand>(expr_member)) {
        operands.push(getValue(std::get<Operand>(expr_member), bool_vector, params_amount_));
      } else {
        Operation operation = std::get<Operation>(expr_member);
        if (operation.operands_amount() == 1) {
          const bool operand_value = operands.top();
          operands.pop();
          auto result_of_calculating = operation(operand_value);
          results_of_calculating.push_back(result_of_calculating);
          operands.push(result_of_calculating);
        } else {
          const bool operand_value_1 = operands.top();
          operands.pop();
          const bool operand_value_2 = operands.top();
          operands.pop();
          auto result_of_calculating = operation(operand_value_2, operand_value_1);
          results_of_calculating.push_back(result_of_calculating);
          operands.push(result_of_calculating);
        }
      }
    }

    return results_of_calculating;
  }
} // BoolCalc