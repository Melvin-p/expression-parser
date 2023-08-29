#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include <map>
#include <memory>
#include <string>

class Parser {
public:
  Parser();
  void operator()(std::string &s);

private:
  std::unique_ptr<Lexer> m_lexer;
  std::map<const std::string, double> m_symbol_table;

  double assignExpr();
  [[nodiscard]] double addExpr();
  double mulExpr();
  double powExpr();
  double unaryExpr();
  double primary();
  double getArgument();

  [[nodiscard]] bool binaryExpr();
  bool binaryUnaryExpr();
  bool binaryCompExpr();
  bool binaryPrimary();

  void checkDomain(double x, double y);
};

#endif