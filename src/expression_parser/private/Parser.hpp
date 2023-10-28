#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "Node.hpp"
#include <memory>

class Parser {
public:
  Parser() = default;

  std::unique_ptr<Program> genAST(std::string &s);

private:
  std::unique_ptr<Lexer> m_lexer;

  std::unique_ptr<Statement> assignExpr();
  std::unique_ptr<Expression> booleanUnaryExpr();
  std::unique_ptr<Expression> booleanExpr();
  std::unique_ptr<Expression> comparisonExpr();
  std::unique_ptr<Expression> addExpr();
  std::unique_ptr<Expression> mulExpr();
  std::unique_ptr<Expression> powExpr();
  std::unique_ptr<Expression> unaryExpr();
  std::unique_ptr<Expression> primary();
  std::unique_ptr<Expression> getArgument();
};

#endif