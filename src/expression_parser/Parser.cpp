#include "Parser.hpp"
#include "Errors.hpp"
#include "Lexer.hpp"
#include "tokens.hpp"
#include <cassert>
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
    var out{};
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
      auto first = std::get_if<bool>(&out);
      auto second = std::get_if<double>(&out);
      if (first != nullptr) {
        m_buffer << std::setprecision(4) << *first;
      } else if (second != nullptr) {
        m_buffer << std::setprecision(4) << *second;
      } else {
        assert(false);
      }
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
    var second = booleanExpr();
    if (m_symbol_table.find(text) == m_symbol_table.end()) {
      m_symbol_table[text] = second;
    } else {
      var first = m_symbol_table[text];
      if (first.index() == second.index()) {
        m_symbol_table[text] = second;
      } else {
        throw RuntimeError{"bad assignment wrong data types", t.getLocation()};
      }
    }
  } else {
    throw SyntaxError{"Missing = after variable name", token_assign.getLocation()};
  }
}

Parser::var Parser::booleanExpr() {
  var result = booleanUnaryExpr();
  for (;;) {
    TokenData token{m_lexer->getCurrentToken()};
    std::string loc{token.getLocation()};
    switch (token.m_token) {
    case Token::And: {
      m_lexer->advance();
      auto temp = booleanUnaryExpr();
      auto first = std::get_if<bool>(&result);
      auto second = std::get_if<bool>(&temp);
      if (first == nullptr || second == nullptr) {
        throw RuntimeError{"bad `and` wrong data types", loc};
      } else {
        result = {*first && *second};
      }
      break;
    }
    case Token::Or: {
      m_lexer->advance();
      auto temp = booleanUnaryExpr();
      auto first = std::get_if<bool>(&result);
      auto second = std::get_if<bool>(&temp);
      if (first == nullptr || second == nullptr) {
        throw RuntimeError{"bad `or` wrong data types", loc};
      } else {
        result = {*first || *second};
      }
      break;
    }
    default: {
      return {result};
    }
    }
  }
}

Parser::var Parser::booleanUnaryExpr() {
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  switch (token.m_token) {
  case Token::Not: {
    m_lexer->advance();
    var result = comparisonExpr();
    auto first = std::get_if<bool>(&result);
    if (first == nullptr) {
      throw RuntimeError{"bad `not` wrong data type", loc};
    } else {
      return {!(*first)};
    }
  }
  default: {
    return comparisonExpr();
  }
  }
}

Parser::var Parser::comparisonExpr() {
  var result = addExpr();
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  switch (token.m_token) {
  case Token::Equal_to: {
    m_lexer->advance();
    auto temp = addExpr();
    auto first = std::get_if<double>(&result);
    auto second = std::get_if<double>(&temp);
    if (first == nullptr || second == nullptr) {
      throw RuntimeError{"bad `equals` wrong data types", loc};
    } else {
      return {*first == *second};
    }
  }
  case Token::Not_equal_to: {
    m_lexer->advance();
    auto temp = addExpr();
    auto first = std::get_if<double>(&result);
    auto second = std::get_if<double>(&temp);
    if (first == nullptr || second == nullptr) {
      throw RuntimeError{"bad `equals` wrong data types", loc};
    } else {
      return {*first != *second};
    }
  }
  case Token::Greater_than: {
    m_lexer->advance();
    auto temp = addExpr();
    auto first = std::get_if<double>(&result);
    auto second = std::get_if<double>(&temp);
    if (first == nullptr || second == nullptr) {
      throw RuntimeError{"bad `equals` wrong data types", loc};
    } else {
      return {*first > *second};
    }
  }
  case Token::Less_than: {
    m_lexer->advance();
    auto temp = addExpr();
    auto first = std::get_if<double>(&result);
    auto second = std::get_if<double>(&temp);
    if (first == nullptr || second == nullptr) {
      throw RuntimeError{"bad `equals` wrong data types", loc};
    } else {
      return {*first < *second};
    }
  }
  default: {
    return {result};
  }
  }
}

Parser::var Parser::addExpr() {
  var result = mulExpr();
  for (;;) {
    TokenData token{m_lexer->getCurrentToken()};
    std::string loc{token.getLocation()};
    switch (token.m_token) {
    case Token::Plus: {
      m_lexer->advance();
      auto temp = mulExpr();
      auto first = std::get_if<double>(&result);
      auto second = std::get_if<double>(&temp);
      if (first == nullptr || second == nullptr) {
        throw RuntimeError{"bad `addition` wrong data types", loc};
      } else {
        result = { *first += *second};
      }
      break;
    }
    case Token::Minus: {
      m_lexer->advance();
      auto temp = mulExpr();
      auto first = std::get_if<double>(&result);
      auto second = std::get_if<double>(&temp);
      if (first == nullptr || second == nullptr) {
        throw RuntimeError{"bad `subtraction` wrong data types", loc};
      } else {
        result = { *first -= *second};
      }
      break;
    }
    default: {
      return result;
    }
    }
  }
}

Parser::var Parser::mulExpr() {
  var result = powExpr();
  for (;;) {
    TokenData token{m_lexer->getCurrentToken()};
    std::string loc{token.getLocation()};
    switch (m_lexer->getCurrentToken().m_token) {
    case Token::Mul: {
      m_lexer->advance();
      auto temp = powExpr();
      auto first = std::get_if<double>(&result);
      auto second = std::get_if<double>(&temp);
      if (first == nullptr || second == nullptr) {
        throw RuntimeError{"bad `multiplication` wrong data types", loc};
      } else {
        result = { *first *= *second};
      };
      break;
    }
    case Token::Div: {
      m_lexer->advance();
      auto temp = powExpr();
      auto first = std::get_if<double>(&result);
      auto second = std::get_if<double>(&temp);
      if (first == nullptr || second == nullptr) {
        throw RuntimeError{"bad `division` wrong data types", loc};
      } else {
        if (*second == 0) {
          throw RuntimeError{"Attempted to divide by zero", loc};
        } else {
          result = {*first / *second};
        }
      }
      break;
    }
    case Token::Mod: {
      m_lexer->advance();
      auto temp = powExpr();
      auto first = std::get_if<double>(&result);
      auto second = std::get_if<double>(&temp);
      if (first == nullptr || second == nullptr) {
        throw RuntimeError{"bad `modulo` wrong data types", loc};
      } else {
        if (*second == 0) {
          throw RuntimeError{"Attempted to divide by zero", loc};
        } else {
          result = {std::fmod(*first, *second)};
        }
      }
      break;
    }
    default: {
      return result;
    }
    }
  }
}

Parser::var Parser::powExpr() {
  var result = unaryExpr();
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  double out{};
  if (token.m_token == Token::Pow) {
    m_lexer->advance();
    auto temp = unaryExpr();
    auto first = std::get_if<double>(&result);
    auto second = std::get_if<double>(&temp);
    if (first == nullptr || second == nullptr) {
      throw RuntimeError{"bad `power` wrong data types", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      out = std::pow(*first, *second);
      if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(out) ||
          std::isinf(out)) {
        throw RuntimeError{"Bad power operation", loc};
      } else {
        return {out};
      }
    }
  } else {
    return result;
  }
}

Parser::var Parser::unaryExpr() {
  TokenData token{m_lexer->getCurrentToken()};
  std::string loc{token.getLocation()};
  switch (token.m_token) {
  case Token::Plus: {
    m_lexer->advance();
    var result = primary();
    auto first = std::get_if<double>(&result);
    if (first == nullptr) {
      throw RuntimeError{"bad `+` wrong data type", loc};
    } else {
      return {+(*first)};
    }
    break;
  }
  case Token::Minus: {
    m_lexer->advance();
    var result = primary();
    auto first = std::get_if<double>(&result);
    if (first == nullptr) {
      throw RuntimeError{"bad `-` wrong data type", loc};
    } else {
      return {-(*first)};
    }
    break;
  }
  default:
    return primary();
  }
}

Parser::var Parser::primary() {
  TokenData t = m_lexer->getCurrentToken();
  std::string text = t.getText();
  var arg{};
  std::string loc{t.getLocation()};
  double result{};
  decltype(std::get_if<double>(&arg)) input{};

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
    return {to_number(text)};
    break;
  case Token::True: {
    m_lexer->advance();
    return {true};
  }
  case Token::False: {
    m_lexer->advance();
    return {false};
  }
  case Token::Lp:
    m_lexer->advance();
    arg = booleanExpr();
    if (m_lexer->getCurrentToken().m_token != Token::Rp) {
      throw SyntaxError{"missing ) after subexpression", loc};
    }
    m_lexer->advance();
    return arg;
    break;
  case Token::Sin:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for sin", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::sin(*input);
      if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
        throw RuntimeError{"Invalid argument to sin", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Cos:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for cos", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::cos(*input);
      if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
        throw RuntimeError{"Invalid argument to cos", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Tan:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for tan", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::tan(*input);
      if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
        throw RuntimeError{"Invalid argument to tan", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Asin:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for asin", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::asin(*input);
      if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
        throw RuntimeError{"Invalid argument to asin", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Acos:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for acos", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::acos(*input);
      if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
        throw RuntimeError{"Invalid argument to acos", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Atan:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for atan", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::atan(*input);
      if (std::isnan(result) || std::isinf(result)) {
        throw RuntimeError{"Invalid argument to atan", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Log:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for log", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::log(*input);
      if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(result) ||
          std::isinf(result)) {
        throw RuntimeError{"Invalid argument to log", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Sqrt:
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for sqrt", loc};
    } else {
      std::feclearexcept(FE_ALL_EXCEPT);
      result = std::sqrt(*input);
      if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
        throw RuntimeError{"Invalid argument to sqrt", loc};
      } else {
        return {result};
      }
    }
    break;
  case Token::Int:
    // round towards zero
    arg = getArgument();
    input = std::get_if<double>(&arg);
    if (input == nullptr) {
      throw RuntimeError{"wrong data type for asin", loc};
    } else {
      if (*input < 0) {
        return {std::ceil(*input)};
      } else {
        return {std::floor(*input)};
      }
    }
    break;
  default:
    throw SyntaxError{"invalid expression", loc};
  }
}

Parser::var Parser::getArgument() {
  m_lexer->advance();
  if (m_lexer->getCurrentToken().m_token != Token::Lp) {
    throw SyntaxError{"missing ( after function name", m_lexer->getCurrentToken().getLocation()};
  }
  m_lexer->advance();
  /*
  currently this function is only used in arithmetic functions so
  for now only parse arithmetic expressions
  */
  var arg = addExpr();
  if (m_lexer->getCurrentToken().m_token != Token::Rp) {
    throw SyntaxError{"missing ) after function argument",
                      m_lexer->getCurrentToken().getLocation()};
  }
  m_lexer->advance();
  return arg;
}
