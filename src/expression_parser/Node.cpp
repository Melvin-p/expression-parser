#include "Node.hpp"

void Program::append(std::unique_ptr<Statement> &&s) { m_statements.push_back(std::move(s)); }

std::string Program::toString(bool braces) {
  std::string out{};
  for (const auto &i : m_statements) {
    out.append(i->toString(braces));
  }
  return out;
}