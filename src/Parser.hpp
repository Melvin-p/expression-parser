#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include <map>
#include <memory>
#include <string>

class Parser {
public:
  Parser();
  double operator()(std::string &s);

private:
  // Lexer *m_lexer{nullptr};
  std::unique_ptr<Lexer> m_lexer;
  std::map<const std::string, double> m_symbol_table;

  [[nodiscard]] double assignExpr();
  double addExpr();
  double mulExpr();
  double powExpr();
  double unaryExpr();
  double primary();
  double getArgument();
  void checkDomain(double x, double y);
};

#endif