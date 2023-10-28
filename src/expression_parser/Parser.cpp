#include "Parser.hpp"
#include "AST.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "Node.hpp"
#include "tokens.hpp"
#include <memory>
#include <string>

std::unique_ptr<Program> Parser::genAST(std::string &s) {
  m_lexer = std::make_unique<Lexer>(std::istringstream{s});
  auto output{std::make_unique<Program>()};
  do {

    output->append(assignExpr());

    auto val = m_lexer->getCurrentToken();
    if (val.m_token != Token::Semicolon) {
      throw SyntaxError{"Missing semicolon", val.getLocation()};
    }

    m_lexer->advance();
  } while (m_lexer->getCurrentToken().m_token != Token::EOF_sym);
  return output;
}

std::unique_ptr<Statement> Parser::assignExpr() {
  // get current token
  TokenData t_initial = m_lexer->getCurrentToken();

  if (t_initial.getToken() == Token::Var) {

    // move to next token should be a variable
    m_lexer->advance();
    TokenData token_identifier = m_lexer->getCurrentToken();
    std::string text = token_identifier.getText();
    if (token_identifier.getToken() != Token::Id) {
      throw SyntaxError{"Missing identifier", token_identifier.getLocation()};
    }

    // move to next token which should be =
    m_lexer->advance();
    TokenData token_assign = m_lexer->getCurrentToken();

    if (token_assign.m_token == Token::Assign) {
      m_lexer->advance();
      return std::make_unique<Assignment>(booleanExpr(), std::move(token_identifier));
    } else {
      throw SyntaxError{"Missing = after variable name", token_assign.getLocation()};
    }
  } else {
    return std::make_unique<Print>(booleanExpr());
  }
}

std::unique_ptr<Expression> Parser::booleanExpr() {
  auto left = booleanUnaryExpr();
  for (;;) {
    TokenData token{m_lexer->getCurrentToken()};
    std::string loc{token.getLocation()};
    switch (token.m_token) {
    case Token::And: {
      m_lexer->advance();
      auto right = booleanUnaryExpr();
      std::unique_ptr<Expression> temp{std::make_unique<BinaryBooleanOperation>(
          std::move(left), std::move(token), std::move(right))};
      left.swap(temp);
      break;
    }
    case Token::Or: {
      m_lexer->advance();
      auto right = booleanUnaryExpr();
      std::unique_ptr<Expression> temp{std::make_unique<BinaryBooleanOperation>(
          std::move(left), std::move(token), std::move(right))};
      left.swap(temp);
      break;
    }
    default: {
      return left;
    }
    }
  }
}

std::unique_ptr<Expression> Parser::booleanUnaryExpr() {
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  switch (token.m_token) {
  case Token::Not: {
    m_lexer->advance();
    auto result = comparisonExpr();
    return std::make_unique<UnaryBooleanOperation>(std::move(result), std::move(token));
  }
  default: {
    return comparisonExpr();
  }
  }
}

std::unique_ptr<Expression> Parser::comparisonExpr() {
  auto left = addExpr();
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  switch (token.m_token) {
  case Token::Equal_to: {
    m_lexer->advance();
    auto right = addExpr();
    return std::make_unique<Comparision>(std::move(left), std::move(token), std::move(right));
  }
  case Token::Not_equal_to: {
    m_lexer->advance();
    auto right = addExpr();
    return std::make_unique<Comparision>(std::move(left), std::move(token), std::move(right));
  }
  case Token::Greater_than: {
    m_lexer->advance();
    auto right = addExpr();
    return std::make_unique<Comparision>(std::move(left), std::move(token), std::move(right));
  }
  case Token::Less_than: {
    m_lexer->advance();
    auto right = addExpr();
    return std::make_unique<Comparision>(std::move(left), std::move(token), std::move(right));
  }
  default: {
    return left;
  }
  }
}

std::unique_ptr<Expression> Parser::addExpr() {
  auto left = mulExpr();
  for (;;) {
    TokenData token{m_lexer->getCurrentToken()};
    std::string loc{token.getLocation()};
    switch (token.m_token) {
    case Token::Plus: {
      m_lexer->advance();
      auto right = mulExpr();
      std::unique_ptr<Expression> temp{std::make_unique<BinaryArithmeticOperation>(
          std::move(left), std::move(token), std::move(right))};
      left.swap(temp);
      break;
    }
    case Token::Minus: {
      m_lexer->advance();
      auto right = mulExpr();
      std::unique_ptr<Expression> temp{std::make_unique<BinaryArithmeticOperation>(
          std::move(left), std::move(token), std::move(right))};
      left.swap(temp);
      break;
    }
    default: {
      return left;
    }
    }
  }
}

std::unique_ptr<Expression> Parser::mulExpr() {
  auto left = powExpr();
  for (;;) {
    TokenData token{m_lexer->getCurrentToken()};
    std::string loc{token.getLocation()};
    switch (m_lexer->getCurrentToken().m_token) {
    case Token::Mul: {
      m_lexer->advance();
      auto right = powExpr();
      std::unique_ptr<Expression> temp{std::make_unique<BinaryArithmeticOperation>(
          std::move(left), std::move(token), std::move(right))};
      left.swap(temp);
      break;
    }
    case Token::Div: {
      m_lexer->advance();
      auto right = powExpr();
      std::unique_ptr<Expression> temp{std::make_unique<BinaryArithmeticOperation>(
          std::move(left), std::move(token), std::move(right))};
      left.swap(temp);
      break;
    }
    case Token::Mod: {
      m_lexer->advance();
      auto right = powExpr();
      std::unique_ptr<Expression> temp{std::make_unique<BinaryArithmeticOperation>(
          std::move(left), std::move(token), std::move(right))};
      left.swap(temp);
      break;
    }
    default: {
      return left;
    }
    }
  }
}

std::unique_ptr<Expression> Parser::powExpr() {
  auto left = unaryExpr();
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  if (token.m_token == Token::Pow) {
    m_lexer->advance();
    auto right = unaryExpr();
    return std::make_unique<BinaryArithmeticOperation>(std::move(left), std::move(token),
                                                       std::move(right));
  } else {
    return left;
  }
}

std::unique_ptr<Expression> Parser::unaryExpr() {
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  switch (token.m_token) {
  case Token::Plus: {
    m_lexer->advance();
    auto result = primary();
    return std::make_unique<UnaryArithmeticOperation>(std::move(result), std::move(token));
    break;
  }
  case Token::Minus: {
    m_lexer->advance();
    auto result = primary();
    return std::make_unique<UnaryArithmeticOperation>(std::move(result), std::move(token));
    break;
  }
  default:
    return primary();
  }
}

std::unique_ptr<Expression> Parser::primary() {
  TokenData t = m_lexer->getCurrentToken();
  std::string text = t.getText();
  std::unique_ptr<Expression> arg{};
  std::string loc{t.getLocation()};

  switch (t.m_token) {
  case Token::Id:
    m_lexer->advance();
    return std::make_unique<Variable>(std::move(t));
    break;
  case Token::Number:
    m_lexer->advance();
    return std::make_unique<AtomicArithmetic>(std::move(t));
    break;
  case Token::True: {
    m_lexer->advance();
    return std::make_unique<AtomicBoolean>(std::move(t));
  }
  case Token::False: {
    m_lexer->advance();
    return std::make_unique<AtomicBoolean>(std::move(t));
  }
  case Token::Lp:
    m_lexer->advance();
    arg = booleanExpr();
    if (m_lexer->getCurrentToken().m_token != Token::Rp) {
      throw SyntaxError{"missing ) after subexpression", loc};
    }
    m_lexer->advance();
    // we can skip Parentheses node?
    return arg;
    break;
  case Token::Sin:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Cos:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Tan:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Asin:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Acos:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Atan:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Log:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Sqrt:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  case Token::Int:
    arg = getArgument();
    return std::make_unique<FunctionArithmetic>(std::move(arg), std::move(t));
    break;
  default:
    throw SyntaxError{"invalid expression", loc};
  }
}

std::unique_ptr<Expression> Parser::getArgument() {
  m_lexer->advance();
  if (m_lexer->getCurrentToken().m_token != Token::Lp) {
    throw SyntaxError{"missing ( after function name", m_lexer->getCurrentToken().getLocation()};
  }
  m_lexer->advance();
  /*
  currently this function is only used in arithmetic functions so
  for now only parse arithmetic expressions
  */
  auto arg = addExpr();
  if (m_lexer->getCurrentToken().m_token != Token::Rp) {
    throw SyntaxError{"missing ) after function argument",
                      m_lexer->getCurrentToken().getLocation()};
  }
  m_lexer->advance();
  return arg;
}
