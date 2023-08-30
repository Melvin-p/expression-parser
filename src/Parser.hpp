#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include <map>
#include <memory>
#include <string>

class Parser {
public:
  Parser();

  void evalArithmetic(std::string &s);
  /*
  TODO
  integrate with arithmetic parser
  type information in symbol table
  */
  bool evalBoolean(std::string &s);

private:
  std::unique_ptr<Lexer> m_lexer;
  std::map<const std::string, double> m_symbol_table;

  void assignExpr();
  [[nodiscard]] double addExpr();
  double mulExpr();
  double powExpr();
  double unaryExpr();
  double primary();
  double getArgument();

  [[nodiscard]] bool booleanExpr();
  bool booleanUnaryExpr();
  bool booleanCompExpr();
  bool booleanPrimary();

  void checkDomain(double x, double y);
};

#endif