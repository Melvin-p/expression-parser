#include "AST.hpp"
#include "Errors.hpp"
#include "Node.hpp"
#include "common.hpp"
#include "tokens.hpp"
#include <cassert>
#include <cfenv>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <utility>

#if !(math_errhandling & MATH_ERREXCEPT)
#error "no floating point exceptions"
#endif

Variable::Variable(TokenData &&token) : m_token(std::move(token)) {
  if (m_token.getToken() != Token::Id) {
    throw SyntaxError{"Not an identifier", m_token.getLocation()};
  }
}

std::string Variable::toString([[maybe_unused]] const bool braces) const {
  return {" " + m_token.getText() + " "};
}

double Variable::evalGetDouble(const SymbolTable &symbol_table) const {
  // check if variable exists and that the type is a double
  if (auto pos{symbol_table.find(m_token.getText())}; pos != symbol_table.end()) {
    if (auto val = std::get_if<double>(&(pos->second))) {
      return *val;
    } else {
      throw RuntimeError{"variable with wrong data type used", m_token.getLocation()};
    }
  } else {
    throw RuntimeError{"variable does not exist yet", m_token.getLocation()};
  }
}

bool Variable::evalGetBool(const SymbolTable &symbol_table) const {
  // check if variable exists and that the type is a bool
  if (auto pos{symbol_table.find(m_token.getText())}; pos != symbol_table.end()) {
    if (auto val = std::get_if<bool>(&(pos->second))) {
      return *val;
    } else {
      throw RuntimeError{"variable with wrong data type used", m_token.getLocation()};
    }
  } else {
    throw RuntimeError{"variable does not exist yet", m_token.getLocation()};
  }
}

var Variable::eval(const SymbolTable &symbol_table) const {
  if (auto pos{symbol_table.find(m_token.getText())}; pos != symbol_table.end()) {
    return pos->second;
  } else {
    throw RuntimeError{"variable does not exist yet", m_token.getLocation()};
  }
};

AtomicArithmetic::AtomicArithmetic(TokenData &&token) : m_token(std::move(token)) {}

std::string AtomicArithmetic::toString([[maybe_unused]] const bool braces) const {
  return m_token.getText();
}

double AtomicArithmetic::evalGetDouble([[maybe_unused]] const SymbolTable &symbol_table) const {
  std::istringstream iss{m_token.getText()};
  double x{};
  iss >> x;
  if (iss.fail()) {
    throw RuntimeError{"Cannot parse literal", m_token.getLocation()};
  }
  return x;
}

var AtomicArithmetic::eval(const SymbolTable &symbol_table) const {
  return evalGetDouble(symbol_table);
}

ParenthesesArithmetic::ParenthesesArithmetic(std::unique_ptr<Expression> &&input, TokenData &&token)
    : m_token(std::move(token)), m_input(dynamic_unique_ptr_cast<Arithmetic>(std::move(input))) {
  if (!m_input) {
    throw SyntaxError{"bad data type in parentheses expected arithmetic got boolean",
                      m_token.getLocation()};
  }
}

std::string ParenthesesArithmetic::toString(const bool braces) const {
  return {"(" + m_input->toString(braces) + ")"};
}

double ParenthesesArithmetic::evalGetDouble(const SymbolTable &symbol_table) const {
  return m_input->evalGetDouble(symbol_table);
}

var ParenthesesArithmetic::eval(const SymbolTable &symbol_table) const {
  return evalGetDouble(symbol_table);
}

BinaryArithmeticOperation::BinaryArithmeticOperation(std::unique_ptr<Expression> &&left,
                                                     ActionTokenData &&token,
                                                     std::unique_ptr<Expression> &&right)
    : m_left(dynamic_unique_ptr_cast<Arithmetic>(std::move(left))), m_token(token),
      m_right(dynamic_unique_ptr_cast<Arithmetic>(std::move(right))) {
  // check token type
  switch (m_token.getToken()) {
  case ActionTokens::Addition:
  case ActionTokens::Subtraction:
  case ActionTokens::Multiplication:
  case ActionTokens::Division:
  case ActionTokens::Modulo:
  case ActionTokens::Power: {
    break;
  }
  default: {
    throw SyntaxError{"binary arithmetic operator not know", m_token.getLocation()};
  }
  }
  if (!m_left || !m_right) {
    throw SyntaxError{"Bad data type for binary arithmetic operation " + m_token.getOperation() +
                          " ",
                      m_token.getLocation()};
  }
}

std::string BinaryArithmeticOperation::toString(const bool braces) const {
  std::string output{m_left->toString(braces) + m_token.getOperator() + m_right->toString(braces)};
  if (braces) {
    return {"(" + output + ")"};
  } else {
    return output;
  }
}

double BinaryArithmeticOperation::evalGetDouble(const SymbolTable &symbol_table) const {
  double left{m_left->evalGetDouble(symbol_table)};
  double right{m_right->evalGetDouble(symbol_table)};
  double result{};
  auto loc{m_token.getLocation()};

  std::feclearexcept(FE_ALL_EXCEPT);
  switch (m_token.getToken()) {
  case ActionTokens::Addition: {
    result = left + right;
    break;
  }
  case ActionTokens::Subtraction: {
    result = left - right;
    break;
  }
  case ActionTokens::Multiplication: {
    result = left * right;
    break;
  }
  case ActionTokens::Division: {
    result = left / right;
    if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(result) ||
        std::isinf(result)) {
      throw RuntimeError{"Bad divide operation", loc};
    }
    break;
  }
  case ActionTokens::Modulo: {
    result = std::fmod(left, right);
    if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(result) ||
        std::isinf(result)) {
      throw RuntimeError{"Bad modulo operation", loc};
    }
    break;
  }
  case ActionTokens::Power: {
    result = std::pow(left, right);
    if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(result) ||
        std::isinf(result)) {
      throw RuntimeError{"Bad power operation", loc};
    }
    break;
  }
  default: {
    // this should be unreachable
    assert(false);
    result = 0;
  }
  }
  return result;
}

var BinaryArithmeticOperation::eval(const SymbolTable &symbol_table) const {
  return evalGetDouble(symbol_table);
}

UnaryArithmeticOperation::UnaryArithmeticOperation(std::unique_ptr<Expression> &&input,
                                                   ActionTokenData &&token)
    : m_input(dynamic_unique_ptr_cast<Arithmetic>(std::move(input))), m_token(token) {
  // check token type
  switch (m_token.getToken()) {
  case ActionTokens::positive:
  case ActionTokens::negative: {
    break;
  }
  default: {
    throw SyntaxError{"unary arithmetic operator not know", m_token.getLocation()};
  }
  }
  if (!m_input) {
    throw SyntaxError{"Bad data type for unary arithmetic operator " + m_token.getOperation(),
                      m_token.getLocation()};
  }
}

std::string UnaryArithmeticOperation::toString(const bool braces) const {
  std::string out{" "};
  out += m_token.getOperator();
  out += m_input->toString(braces);
  if (braces) {
    return {"(" + out + ")"};
  } else {
    return out;
  }
}

double UnaryArithmeticOperation::evalGetDouble(const SymbolTable &symbol_table) const {
  double input{m_input->evalGetDouble(symbol_table)};
  switch (m_token.getToken()) {
  case ActionTokens::positive:
    return +input;
  case ActionTokens::negative:
    return -input;
  default:
    // this should be unreachable
    assert(false);
    return 0;
  }
}

var UnaryArithmeticOperation::eval(const SymbolTable &symbol_table) const {
  return evalGetDouble(symbol_table);
}

FunctionArithmetic::FunctionArithmetic(std::unique_ptr<Expression> &&input, ActionTokenData &&token)
    : m_input(dynamic_unique_ptr_cast<Arithmetic>(std::move(input))), m_token(token) {
  switch (m_token.getToken()) {
  case ActionTokens::sin:
  case ActionTokens::cos:
  case ActionTokens::tan:
  case ActionTokens::Atan:
  case ActionTokens::Acos:
  case ActionTokens::Asin:
  case ActionTokens::Log:
  case ActionTokens::Sqrt:
  case ActionTokens::Int: {
    break;
  }
  default: {
    throw SyntaxError{"function call not found", m_token.getLocation()};
  }
  }
  if (!m_input) {
    throw SyntaxError{"Bad data type when calling function", m_token.getLocation()};
  }
}

std::string FunctionArithmetic::toString(const bool braces) const {
  switch (m_token.getToken()) {
  case ActionTokens::sin: {
    return {" sin(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::cos: {
    return {" cos(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::tan: {
    return {" tan(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::Atan: {
    return {" atan(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::Acos: {
    return {" acos(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::Asin: {
    return {" asin(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::Log: {
    return {" log(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::Sqrt: {
    return {" sqrt(" + m_input->toString(braces) + ")"};
  }
  case ActionTokens::Int: {
    return {" Int(" + m_input->toString(braces) + ")"};
  }
  default: {
    assert(false);
    return {""};
  }
  }
}

double FunctionArithmetic::evalGetDouble(const SymbolTable &symbol_table) const {
  double input{m_input->evalGetDouble(symbol_table)};
  double result{};
  auto loc{m_token.getLocation()};

  std::feclearexcept(FE_ALL_EXCEPT);

  switch (m_token.getToken()) {
  case ActionTokens::sin: {
    result = std::sin(input);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to sin", loc};
    }
    break;
  }
  case ActionTokens::cos: {
    result = std::cos(input);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to cos", loc};
    }
    break;
  }
  case ActionTokens::tan: {
    result = std::tan(input);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to tan", loc};
    }
    break;
  }
  case ActionTokens::Atan: {
    result = std::atan(input);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to atan", loc};
    }
    break;
  }
  case ActionTokens::Acos: {
    result = std::acos(input);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to acos", loc};
    }
    break;
  }
  case ActionTokens::Asin: {
    result = std::asin(input);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to asin", loc};
    }
    break;
  }
  case ActionTokens::Log: {
    result = std::log(input);
    if (std::fetestexcept(FE_INVALID) || std::fetestexcept(FE_DIVBYZERO) || std::isnan(result) ||
        std::isinf(result)) {
      throw RuntimeError{"Invalid argument to log", loc};
    }
    break;
  }
  case ActionTokens::Sqrt: {
    result = std::sqrt(input);
    if (std::fetestexcept(FE_INVALID) || std::isnan(result) || std::isinf(result)) {
      throw RuntimeError{"Invalid argument to sqrt", loc};
    }
    break;
  }
  case ActionTokens::Int: {
    if (input < 0) {
      result = std::ceil(input);
    } else {
      result = std::floor(input);
    }
    break;
  }
  default:
    // this should be unreachable
    assert(false);
    result = 0;
  }

  return result;
}

var FunctionArithmetic::eval(const SymbolTable &symbol_table) const {
  return evalGetDouble(symbol_table);
}

AtomicBoolean::AtomicBoolean(TokenData &&token) : m_token(token) {
  switch (m_token.getToken()) {
  case Token::True:
  case Token::False:
    break;
  default:
    throw SyntaxError{"Boolean required", m_token.getLocation()};
  }
};

std::string AtomicBoolean::toString([[maybe_unused]] const bool braces) const {
  if (m_token.getToken() == Token::True) {
    return {" true"};
  } else if (m_token.getToken() == Token::False) {
    return {" false "};
  } else {
    assert(false);
    return {""};
  }
}

bool AtomicBoolean::evalGetBool([[maybe_unused]] const SymbolTable &symbol_table) const {
  if (m_token.getToken() == Token::True) {
    return true;
  } else if (m_token.getToken() == Token::False) {
    return false;
  } else {
    // should be unreachable
    assert(false);
    return false;
  }
}

var AtomicBoolean::eval(const SymbolTable &symbol_table) const { return evalGetBool(symbol_table); }

ParenthesesBoolean::ParenthesesBoolean(std::unique_ptr<Expression> &&input, TokenData &&token)
    : m_token(std::move(token)), m_input(dynamic_unique_ptr_cast<Boolean>(std::move(input))) {
  if (!m_input) {
    throw SyntaxError{"bad data type in parentheses expected boolean got something else",
                      m_token.getLocation()};
  }
}

std::string ParenthesesBoolean::toString(const bool braces) const {
  return {"(" + m_input->toString(braces) + ")"};
}

bool ParenthesesBoolean::evalGetBool(const SymbolTable &symbol_table) const {
  return m_input->evalGetBool(symbol_table);
}

var ParenthesesBoolean::eval(const SymbolTable &symbol_table) const {
  return evalGetBool(symbol_table);
}

BinaryBooleanOperation::BinaryBooleanOperation(std::unique_ptr<Expression> &&left,
                                               ActionTokenData &&token,
                                               std::unique_ptr<Expression> &&right)
    : m_left(dynamic_unique_ptr_cast<Boolean>(std::move(left))), m_token(token),
      m_right(dynamic_unique_ptr_cast<Boolean>(std::move(right))) {
  switch (m_token.getToken()) {
  case ActionTokens::And:
  case ActionTokens::Or: {
    break;
  }
  default: {
    throw SyntaxError{"Boolean operation not know", m_token.getLocation()};
  }
  }
  if (!m_left || !m_right) {
    throw SyntaxError{"Bad data types for boolean operation", m_token.getLocation()};
  }
}

std::string BinaryBooleanOperation::toString(const bool braces) const {
  std::string output;
  switch (m_token.getToken()) {

  case ActionTokens::And: {
    output = {m_left->toString(braces) + " and " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case ActionTokens::Or: {
    output = {m_left->toString(braces) + " or " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  default: {
    assert(false);
    return {""};
  }
  }
}

bool BinaryBooleanOperation::evalGetBool(const SymbolTable &symbol_table) const {
  bool left{m_left->evalGetBool(symbol_table)};
  bool right{m_right->evalGetBool(symbol_table)};
  switch (m_token.getToken()) {
  case ActionTokens::And:
    return left && right;
  case ActionTokens::Or:
    return left || right;
  default:
    // this should be unreachable
    assert(false);
    return false;
  }
}

var BinaryBooleanOperation::eval(const SymbolTable &symbol_table) const {
  return evalGetBool(symbol_table);
}

UnaryBooleanOperation::UnaryBooleanOperation(std::unique_ptr<Expression> &&input,
                                             ActionTokenData &&token)
    : m_input(dynamic_unique_ptr_cast<Boolean>(std::move(input))), m_token(token) {
  switch (m_token.getToken()) {
  case ActionTokens::Not: {
    break;
  }
  default: {
    throw SyntaxError{"Boolean operation not know", m_token.getLocation()};
  }
  }
  if (!m_input) {
    throw SyntaxError{"Bad data types for boolean operation", m_token.getLocation()};
  }
}

std::string UnaryBooleanOperation::toString(const bool braces) const {
  std::string output{m_input->toString(braces)};
  // currently the only unary boolean token
  if (braces) {
    return {" not (" + output + ")"};
  } else {
    return {" not " + output};
  }
}

bool UnaryBooleanOperation::evalGetBool(const SymbolTable &symbol_table) const {
  bool input{m_input->evalGetBool(symbol_table)};
  return !input;
}

var UnaryBooleanOperation::eval(const SymbolTable &symbol_table) const {
  return evalGetBool(symbol_table);
}

Comparision::Comparision(std::unique_ptr<Expression> &&left, ActionTokenData &&token,
                         std::unique_ptr<Expression> &&right)
    : m_left(dynamic_unique_ptr_cast<Arithmetic>(std::move(left))), m_token(token),
      m_right(dynamic_unique_ptr_cast<Arithmetic>(std::move(right))) {
  switch (m_token.getToken()) {
  case ActionTokens::Greater_than:
  case ActionTokens::Less_than:
  case ActionTokens::Equal_to:
  case ActionTokens::Not_equal_to: {
    break;
  }
  default: {
    throw SyntaxError{"comparison operator not know", m_token.getLocation()};
  }
  }
  if (!m_left || !m_right) {
    throw SyntaxError{"Bad data type for comparison operation " + m_token.getOperation() + " ",
                      m_token.getLocation()};
  }
}

std::string Comparision::toString(const bool braces) const {
  std::string output;
  switch (m_token.getToken()) {
  case ActionTokens::Greater_than: {
    output = {m_left->toString(braces) + " greater_than " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case ActionTokens::Less_than: {
    output = {m_left->toString(braces) + " less_than " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case ActionTokens::Equal_to: {
    output = {m_left->toString(braces) + " equal_to " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case ActionTokens::Not_equal_to: {
    output = {m_left->toString(braces) + " not_equal_to " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  default: {
    assert("");
    return {""};
  }
  }
}

bool Comparision::evalGetBool(const SymbolTable &symbol_table) const {
  double left{m_left->evalGetDouble(symbol_table)};
  double right{m_right->evalGetDouble(symbol_table)};
  switch (m_token.getToken()) {
  case ActionTokens::Greater_than:
    return left > right;
  case ActionTokens::Less_than:
    return left < right;
  case ActionTokens::Equal_to:
    return left == right;
  case ActionTokens::Not_equal_to:
    return left != right;
  default:
    // this should be unreachable
    assert(false);
    return false;
  }
}

var Comparision::eval(const SymbolTable &symbol_table) const { return evalGetBool(symbol_table); }

Assignment::Assignment(std::unique_ptr<Expression> &&value, TokenData &&token, bool create_var)
    : m_token(token), m_value(std::move(value)), m_create_var(create_var) {
  if (m_token.getToken() != Token::Id) {
    throw SyntaxError{"no variable to assign to", m_token.getLocation()};
  }
}

std::string Assignment::toString(const bool braces) const {
  if (m_create_var) {
    return {"var " + m_token.getText() + " = " + m_value->toString(braces) + ";\n"};
  } else {
    return {" " + m_token.getText() + " = " + m_value->toString(braces) + ";\n"};
  }
}

std::string Assignment::evalGetString(SymbolTable &symbol_table) const {
  auto variable_name = m_token.getText();

  if (variable_name == "pi" || variable_name == "e" || variable_name == "nan" ||
      variable_name == "inf") {
    throw SyntaxError{"Attempted to modify built in constants", m_token.getLocation()};
  }

  auto pos{symbol_table.find(variable_name)};
  bool var_exists = pos != symbol_table.end();

  auto assignment_value = m_value->eval(symbol_table);

  // variable exists and not trying to create new variable
  if (var_exists && !m_create_var) {

    auto variable_value = pos->second;

    if (variable_value.index() == assignment_value.index()) {
      symbol_table[variable_name] = assignment_value;
    } else {
      throw RuntimeError{"attempted to assign wrong data type to variable", m_token.getLocation()};
    }
  }
  // variable already exists and trying to create new variable
  else if (var_exists && m_create_var) {
    throw RuntimeError{"Tried to create already existing variable", m_token.getLocation()};
  }
  // variable does not exist and trying to create new variable
  else if (!var_exists && m_create_var) {
    symbol_table[variable_name] = assignment_value;
  }
  // variable does not exist and not trying to create new variable
  else {
    throw RuntimeError{"Unkown variable", m_token.getLocation()};
  }
  return "";
}

std::string Print::evalGetString(SymbolTable &symbol_table) const {

  var result = m_value->eval(symbol_table);

  if (auto value = std::get_if<bool>(&result)) {

    if (*value) {
      return "true\n";
    } else {
      return "false\n";
    }
  } else if (auto value = std::get_if<double>(&result)) {

    std::stringstream buffer;
    buffer << std::setprecision(4) << *value;
    auto val{buffer.str() + '\n'};
    return val;

  } else {
    assert(false);
    return "";
  }
}

std::string Print::toString(const bool braces) const { return {m_value->toString(braces) + ";\n"}; }
