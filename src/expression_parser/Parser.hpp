#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <sstream>
#include <unordered_map>
#include <variant>

class Parser {
public:
  Parser();

  void evalArithmetic(std::string &s);
  std::string getResult();
  friend std::ostream &operator<<(std::ostream &os, Parser &parser);
  friend void operator>>(std::string &s, Parser &parser);

private:
  using var = std::variant<bool, double>;
  std::unordered_map<std::string, var> m_symbol_table{};
  std::stringstream m_buffer{};

  void assignExpr();
  var booleanUnaryExpr();
  var booleanExpr();
  var comparisonExpr();
  var addExpr();
  var mulExpr();
  var powExpr();
  var unaryExpr();
  var primary();
  var getArgument();
};

#endif