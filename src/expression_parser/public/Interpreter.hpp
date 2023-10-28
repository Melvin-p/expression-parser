#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <unordered_map>
#include <variant>

class Interpreter {
private:
  using var = std::variant<bool, double>;
  std::unordered_map<std::string, var> m_symbol_table{};

public:
  Interpreter();
  [[nodiscard]] std::string evaluate(std::string &s);
  void reset();
};

#endif