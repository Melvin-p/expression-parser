#include "Parser.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include <cmath>
#include <sstream>

namespace {
inline double to_number(std::string &s) {
  std::istringstream iss{s};
  iss.exceptions(std::ios::failbit);
  double x{};
  iss >> x;
  return x;
}
} // namespace

Parser::Parser() {
  m_symbol_table["pi"] = 4.0 * std::atan(1.0);
  m_symbol_table["e"] = std::exp(1.0);
}

double Parser::operator()(std::string &s) {
  m_lexer = std::make_unique<Lexer>(std::istringstream{s});
  double result{};
  do {
    result = assignExpr();
  } while (m_lexer->getCurrentToken() != Token::EOF_sym);
  return result;
}

double Parser::assignExpr() {
  Token t = m_lexer->getCurrentToken();
  std::string text = m_lexer->getCurrentTokenText();

  double result = addExpr();

  if (m_lexer->getCurrentToken() == Token::Assign) {
    auto loc = m_lexer->getLocation();
    if (t != Token::Id) {
      throw SyntaxError{"Target of assignment must be an identifier", loc};
    }
    if (text == "pi" || text == "e") {
      throw SyntaxError{"Attempted to modify built in constants", loc};
    }
    m_lexer->advance();
    return m_symbol_table[text] = addExpr();
  }
  return result;
}

double Parser::addExpr() {
  double result = mulExpr();
  for (;;) {
    switch (m_lexer->getCurrentToken()) {
    case Token::Plus: {
      m_lexer->advance();
      result += mulExpr();
      break;
    }
    case Token::Minus: {
      m_lexer->advance();
      result -= mulExpr();
      break;
    }
    default: {
      return result;
    }
    }
  }
}

double Parser::mulExpr() {
  double result = powExpr();
  double x{};
  for (;;) {
    std::string loc;
    switch (m_lexer->getCurrentToken()) {
    case Token::Mul: {
      m_lexer->advance();
      result *= powExpr();
      break;
    }
    case Token::Div: {
      m_lexer->advance();
      x = powExpr();
      if (x == 0) {
        loc = m_lexer->getLocation();
        throw RuntimeError{"Attempted to divide by zero", loc};
      }
      result /= x;
      break;
    }
    case Token::Mod: {
      m_lexer->advance();
      x = powExpr();
      if (x == 0) {
        loc = m_lexer->getLocation();
        throw RuntimeError{"Attempted to divide by zero", loc};
      }
      result = std::fmod(result, x);
      break;
    }
    default: {
      return result;
    }
    }
  }
}

double Parser::powExpr() {
  double result = unaryExpr();
  if (m_lexer->getCurrentToken() == Token::Pow) {
    m_lexer->advance();
    double x = unaryExpr();
    checkDomain(result, x);
    result = std::pow(result, x);
  }
  return result;
}

double Parser::unaryExpr() {
  switch (m_lexer->getCurrentToken()) {
  case Token::Plus: {
    m_lexer->advance();
    return +primary();
    break;
  }
  case Token::Minus: {
    m_lexer->advance();
    return -primary();
    break;
  }
  default:
    return primary();
  }
}

double Parser::primary() {
  std::string text = m_lexer->getCurrentTokenText();
  double arg{};
  std::string loc;

  switch (m_lexer->getCurrentToken()) {
  case Token::Id:
    m_lexer->advance();
    return m_symbol_table[text];
    break;
  case Token::Number:
    m_lexer->advance();
    return to_number(text);
    break;
  case Token::Lp:
    m_lexer->advance();
    arg = addExpr();
    loc = m_lexer->getLocation();
    if (m_lexer->getCurrentToken() != Token::Rp) {
      throw SyntaxError{"missing ) after subexpression", loc};
    }
    m_lexer->advance();
    return arg;
    break;
  case Token::Sin:
    return std::sin(getArgument());
    break;
  case Token::Cos:
    return std::cos(getArgument());
    break;
  case Token::Tan:
    arg = getArgument();
    loc = m_lexer->getLocation();
    if (cos(arg) == 0) {
      throw RuntimeError{"Invalid argument to tan", loc};
    }
    return std::tan(arg);
    break;
  case Token::Asin:
    return std::asin(getArgument());
    break;
  case Token::Acos:
    return std::acos(getArgument());
    break;
  case Token::Atan:
    return std::atan(getArgument());
    break;
  case Token::Log:
    arg = getArgument();
    loc = m_lexer->getLocation();
    if (arg < 1) {
      throw RuntimeError{"Invalid Argument to log", loc};
    }
    return std::log(arg);
    break;
  case Token::Sqrt:
    arg = getArgument();
    loc = m_lexer->getLocation();
    if (arg < 0) {
      throw RuntimeError{"Invalid argument to sqrt", loc};
    }
    return std::sqrt(arg);
    break;
  case Token::Int:
    arg = getArgument();
    if (arg < 0) {
      return std::ceil(arg);
    } else {
      return std::floor(arg);
    }
    break;
  default:
    loc = m_lexer->getLocation();
    throw SyntaxError{"invalid expression", loc};
  }
}

double Parser::getArgument() {
  m_lexer->advance();
  std::string loc;
  if (m_lexer->getCurrentToken() != Token::Lp) {
    loc = m_lexer->getLocation();
    throw SyntaxError{"missing ( after function name", loc};
  }
  m_lexer->advance();
  double arg = addExpr();
  if (m_lexer->getCurrentToken() != Token::Rp) {
    loc = m_lexer->getLocation();
    throw SyntaxError{"missing ) after function argument", loc};
  }
  m_lexer->advance();
  return arg;
}

void Parser::checkDomain(double x, double y) {
  if (x >= 0) {
    return;
  }
  double e = std::abs(y);
  if (e <= 0 || e >= 1) {
    return;
  }
  std::string loc = m_lexer->getLocation();
  throw RuntimeError{"attempted to take root of a negative number", loc};
}

bool Parser::binaryExpr() {
  bool result = binaryUnaryExpr();
  for (;;) {
    switch (m_lexer->getCurrentToken()) {
    case Token::And: {
      m_lexer->advance();
      auto temp = binaryUnaryExpr();
      result = result && temp;
      break;
    }
    case Token::Or: {
      m_lexer->advance();
      auto temp = binaryUnaryExpr();
      result = result || temp;
      break;
    }
    default: {
      return result;
    }
    }
  }
}

bool Parser::binaryUnaryExpr() {
  switch (m_lexer->getCurrentToken()) {
  case Token::Not: {
    m_lexer->advance();
    return !binaryPrimary();
  }
  default: {
    return binaryPrimary();
  }
  }
}

bool Parser::binaryPrimary() {
  bool arg{};
  std::string loc;
  switch (m_lexer->getCurrentToken()) {
  case Token::True: {
    m_lexer->advance();
    return true;
  }
  case Token::False: {
    m_lexer->advance();
    return false;
  }
  case Token::Lp: {
    m_lexer->advance();
    arg = binaryExpr();
    loc = m_lexer->getLocation();
    if (m_lexer->getCurrentToken() != Token::Rp) {
      throw SyntaxError{"missing ) after subexpression", loc};
    }
    m_lexer->advance();
    return arg;
  }
  case Token::Number: {
    arg = binaryCompExpr();
    return arg;
  }
  default:
    loc = m_lexer->getLocation();
    throw SyntaxError{"invalid expression", loc};
  }
}

bool Parser::binaryCompExpr() {
  std::string loc;

  double result = addExpr();

  switch (m_lexer->getCurrentToken()) {
  case Token::Equal_to: {
    m_lexer->advance();
    return result == addExpr();
  }
  case Token::Not_equal_to: {
    m_lexer->advance();
    return result != addExpr();
  }
  case Token::Greater_than: {
    m_lexer->advance();
    return result > addExpr();
  }
  case Token::Less_than: {
    m_lexer->advance();
    return result < addExpr();
  }
  default: {
    loc = m_lexer->getLocation();
    throw SyntaxError{"invalid comparator", loc};
  }
  }
}
