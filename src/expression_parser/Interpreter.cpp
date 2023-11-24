#include "Interpreter.hpp"
#include "Parser.hpp"
#include <cmath>
#include <limits>

Interpreter::Interpreter() {
  m_symbol_table["pi"] = 4.0 * std::atan(1.0);
  m_symbol_table["e"] = std::exp(1.0);
  m_symbol_table["nan"] = std::numeric_limits<double>::quiet_NaN();
  m_symbol_table["inf"] = std::numeric_limits<double>::infinity();
}

std::string Interpreter::evaluate(std::string &s) {
  Parser parser{};
  auto val = parser.genAST(s);
  return val->eval(m_symbol_table);
}

const SymbolTable &Interpreter::getSymbolTable() const { return m_symbol_table; }

void Interpreter::reset() {
  m_symbol_table.clear();
  m_symbol_table["pi"] = 4.0 * std::atan(1.0);
  m_symbol_table["e"] = std::exp(1.0);
  m_symbol_table["nan"] = std::numeric_limits<double>::quiet_NaN();
  m_symbol_table["inf"] = std::numeric_limits<double>::infinity();
}