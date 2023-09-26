#include "Parser.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "tokens.hpp"
#include <cfenv>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#if !(math_errhandling & MATH_ERREXCEPT)
#error "no floating point exceptions"
#endif

namespace {
inline double to_number(std::string &s) {
  std::istringstream iss{s};
  iss.exceptions(std::ios::failbit);
  double x{};
  iss >> x;
  return x;
}
std::unique_ptr<Lexer> m_lexer;
} // namespace

Parser::Parser() {
  m_symbol_table["pi"] = 4.0 * std::atan(1.0);
  m_symbol_table["e"] = std::exp(1.0);
  m_symbol_table["nan"] = std::numeric_limits<double>::quiet_NaN();
  m_symbol_table["inf"] = std::numeric_limits<double>::infinity();
}

std::string Parser::getResult() {
  std::string out{m_buffer.str()};
  m_buffer.str(std::string{});
  return out;
}

std::ostream &operator<<(std::ostream &os, Parser &parser) {
  os << parser.m_buffer.str();
  parser.m_buffer.str(std::string{});
  return os;
}

void operator>>(std::string &s, Parser &parser) { parser.evalArithmetic(s); }

void Parser::evalArithmetic(std::string &s) {
  m_lexer = std::make_unique<Lexer>(std::istringstream{s});
  do {
    double out{};
    bool output{false};
    switch (m_lexer->getCurrentToken().m_token) {
    case Token::Var: {
      m_lexer->advance();
      auto val = m_lexer->getCurrentToken();
      if (val.m_token != Token::Id) {
        throw SyntaxError{"Missing identifier", val.getLocation()};
      }
      assignExpr();
      break;
    }
    case Token::Print: {
      out = getArgument();
      output = true;
      break;
    }
    default: {
      out = booleanExpr();
      output = true;
      break;
    }
    }
    auto val = m_lexer->getCurrentToken();
    if (val.m_token != Token::Semicolon) {
      throw SyntaxError{"Missing semicolon", val.getLocation()};
    }
    if (output) {
      m_buffer << std::setprecision(4) << out;
      m_buffer << "\n";
    }
    m_lexer->advance();
  } while (m_lexer->getCurrentToken().m_token != Token::EOF_sym);
}

void Parser::assignExpr() {
  // get current token should be a variable
  TokenData t = m_lexer->getCurrentToken();
  std::string text = t.getText();

  // move to next token should be a assignment operator
  m_lexer->advance();

  TokenData token_assign = m_lexer->getCurrentToken();
  if (token_assign.m_token == Token::Assign) {

    if (t.m_token != Token::Id) {
      throw SyntaxError{"Target of assignment must be an identifier", t.getLocation()};
    }
    if (text == "pi" || text == "e" || text == "nan" || text == "inf") {
      throw SyntaxError{"Attempted to modify built in constants", t.getLocation()};
    }
    m_lexer->advance();
    m_symbol_table[text] = booleanExpr();
  } else {
    throw SyntaxError{"Missing = after variable name", token_assign.getLocation()};
  }
}

double Parser::booleanExpr() {
  double result = booleanUnaryExpr();
  for (;;) {
    switch (m_lexer->getCurrentToken().m_token) {
    case Token::And: {
      m_lexer->advance();
      auto temp = booleanUnaryExpr();
      result = result && temp;
      break;
    }
    case Token::Or: {
      m_lexer->advance();
      auto temp = booleanUnaryExpr();
      result = result || temp;
      break;
    }
    default: {
      return result;
    }
    }
  }
}

double Parser::booleanUnaryExpr() {
  switch (m_lexer->getCurrentToken().m_token) {
  case Token::Not: {
    m_lexer->advance();
    return !comparisonExpr();
  }
  default: {
    return comparisonExpr();
  }
  }
}

double Parser::comparisonExpr() {
  std::string loc;

  double result = addExpr();

  switch (m_lexer->getCurrentToken().m_token) {
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
    return result;
  }
  }
}

double Parser::addExpr() {
  double result = mulExpr();
  for (;;) {
    switch (m_lexer->getCurrentToken().m_token) {
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
    switch (m_lexer->getCurrentToken().m_token) {
    case Token::Mul: {
      m_lexer->advance();
      result *= powExpr();
      break;
    }
    case Token::Div: {
      m_lexer->advance();
      x = powExpr();
      if (x == 0) {
        loc = m_lexer->getCurrentToken().getLocation();
        throw RuntimeError{"Attempted to divide by zero", loc};
      }
      result /= x;
      break;
    }
    case Token::Mod: {
      m_lexer->advance();
      x = powExpr();
      if (x == 0) {
        loc = m_lexer->getCurrentToken().getLocation();
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
  if (m_lexer->getCurrentToken().m_token == Token::Pow) {
    std::string loc{m_lexer->getCurrentToken().getLocation()};
    m_lexer->advance();
    double x = unaryExpr();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::pow(result, x);
    if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(result) ||
        std::isinf(result)) {
      throw RuntimeError{"Bad power operation", loc};
    }
  }
  return result;
}

double Parser::unaryExpr() {
  switch (m_lexer->getCurrentToken().m_token) {
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
  TokenData t = m_lexer->getCurrentToken();
  std::string text = t.getText();
  double arg{};
  std::string loc{t.getLocation()};
  double result{};

  switch (t.m_token) {
  case Token::Id:
    m_lexer->advance();
    if (m_symbol_table.find(text) != m_symbol_table.end()) {
      return m_symbol_table[text];
    } else {
      throw RuntimeError{"variable does not exist yet", loc};
    }
    break;
  case Token::Number:
    m_lexer->advance();
    return to_number(text);
    break;
  case Token::True: {
    m_lexer->advance();
    return true;
  }
  case Token::False: {
    m_lexer->advance();
    return false;
  }
  case Token::Lp:
    m_lexer->advance();
    arg = booleanUnaryExpr();
    loc = m_lexer->getCurrentToken().getLocation();
    if (m_lexer->getCurrentToken().m_token != Token::Rp) {
      throw SyntaxError{"missing ) after subexpression", loc};
    }
    m_lexer->advance();
    return arg;
    break;
  case Token::Sin:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::sin(arg);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to sin", loc};
    } else {
      return result;
    }
    break;
  case Token::Cos:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::cos(arg);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to cos", loc};
    } else {
      return result;
    }
    break;
  case Token::Tan:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::tan(arg);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to tan", loc};
    } else {
      return result;
    }
    break;
  case Token::Asin:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::asin(arg);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to asin", loc};
    } else {
      return result;
    }
    break;
  case Token::Acos:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::acos(arg);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to acos", loc};
    } else {
      return result;
    }
    break;
  case Token::Atan:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::atan(arg);
    if (std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to atan", loc};
    } else {
      return result;
    }
    break;
  case Token::Log:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::log(arg);
    if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(result) ||
        std::isinf(result)) {
      throw RuntimeError{"Invalid argument to log", loc};
    } else {
      return result;
    }
    break;
  case Token::Sqrt:
    arg = getArgument();
    std::feclearexcept(FE_ALL_EXCEPT);
    result = std::sqrt(arg);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to sqrt", loc};
    } else {
      return result;
    }
    break;
  case Token::Int:
    // round towards zero
    arg = getArgument();
    if (arg < 0) {
      return std::ceil(arg);
    } else {
      return std::floor(arg);
    }
    break;
  default:
    throw SyntaxError{"invalid expression", loc};
  }
}

double Parser::getArgument() {
  m_lexer->advance();
  std::string loc;
  if (m_lexer->getCurrentToken().m_token != Token::Lp) {
    throw SyntaxError{"missing ( after function name", m_lexer->getCurrentToken().getLocation()};
  }
  m_lexer->advance();
  double arg = addExpr();
  if (m_lexer->getCurrentToken().m_token != Token::Rp) {
    throw SyntaxError{"missing ) after function argument",
                      m_lexer->getCurrentToken().getLocation()};
  }
  m_lexer->advance();
  return arg;
}
