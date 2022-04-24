#ifndef MAIN_CPP_REVERSE_POLISH_NOTATION_HPP
#define MAIN_CPP_REVERSE_POLISH_NOTATION_HPP

#include <iostream>
#include <exception>
#include <functional>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <variant>

namespace BoolCalc {

  enum class MemberType {
    Operand,
    Operation
  };

  class Operation {
  public:
    enum class Type {
      OpenBracket,
      CloseBracket,
      Equality,
      Inequality,
      Sub,
      StrictSub,
      Not,
      And,
      Or,
      Diff,
      Xor
    };

    enum class Association {
      Left,
      Right
    };

  private:
    Type type_;
    uint8_t operands_amount_;
    Association association_;
    uint8_t priority_;
    size_t index_ = 0;
    std::function<bool(bool,
                       bool)> function_;
    std::string presenting_symbol_;

  public:
    Operation() = default;

    explicit Operation(char presenting_symbol);

    [[nodiscard]] Type type() const;

    [[nodiscard]] Association association() const;

    [[nodiscard]] uint8_t operands_amount() const;

    [[nodiscard]]  const std::string& presenting_symbol() const;

    [[nodiscard]] uint8_t priority() const;

    void index(size_t index);

    [[nodiscard]] size_t index() const;

    bool operator()(bool a,
                    bool b);
  };

  const std::unordered_map<char, Operation::Type>& operations_dictionary();

  class Operand {
    uint8_t index_;
  public:
    Operand() = default;

    explicit Operand(char presenting_symbol);

    [[nodiscard]] uint8_t index() const;
  };

  class ReversePolishNotation {
    std::vector<std::variant<Operation, Operand>> input_expression_;
    std::vector<std::variant<Operation, Operand>> out_expression_;
    uint8_t params_amount_ = 0;

    void getLastOperation(std::stack<std::reference_wrapper<Operation>>& operations,
                            size_t& operation_index);

  public:
    explicit ReversePolishNotation(const std::string& input_expression);

    [[nodiscard]] uint8_t params_amount() const;

    [[nodiscard]]  std::vector<bool> calculate(size_t bool_vector) const;

    [[nodiscard]] const std::vector<std::variant<Operation, Operand>>& input_expression() const;
  };
} // BoolCalc

#endif //MAIN_CPP_REVERSE_POLISH_NOTATION_HPP
