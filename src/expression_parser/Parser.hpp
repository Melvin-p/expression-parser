#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <sstream>
#include <unordered_map>

class Parser {
public:
  Parser();

  void evalArithmetic(std::string &s);
  std::string getResult();
  friend std::ostream &operator<<(std::ostream &os, Parser &parser);
  friend void operator>>(std::string &s, Parser &parser);

private:
  std::unordered_map<std::string, double> m_symbol_table{};
  std::stringstream m_buffer{};

  void assignExpr();
  double booleanUnaryExpr();
  double booleanExpr();
  double comparisonExpr();
  double addExpr();
  double mulExpr();
  double powExpr();
  double unaryExpr();
  double primary();
  double getArgument();
};

#endif