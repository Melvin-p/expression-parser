#include "Node.hpp"

void Program::append(std::unique_ptr<Statement> &&s) { m_statements.push_back(std::move(s)); }

std::string Program::toString(const bool braces) const {
  std::string out{};
  for (const auto &i : m_statements) {
    out.append(i->toString(braces));
  }
  return out;
}

std::string Program::eval(SymbolTable &symbol_table) {
  std::string buffer{};
  for (const auto &i : m_statements) {
    buffer.append(i->evalGetString(symbol_table));
    if (*(buffer.end() - 1) != '\n') {
      buffer.append("\n");
    }
  }
  return buffer;
}