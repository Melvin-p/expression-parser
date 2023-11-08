#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Types.hpp"
#include <string>

class Interpreter {
private:
  SymbolTable m_symbol_table{};

public:
  Interpreter();
  [[nodiscard]] std::string evaluate(std::string &s);
  void reset();
};

#endif