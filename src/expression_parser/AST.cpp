#include "AST.hpp"
#include "Errors.hpp"
#include "Node.hpp"
#include "tokens.hpp"
#include <cassert>
#include <string>

std::string ParenthesesArithmetic::toString(bool braces) {
  return {"(" + m_input->toString(braces) + ")"};
}

std::string ParenthesesBoolean::toString(bool braces) {
  return {"(" + m_input->toString(braces) + ")"};
}

std::string AtomicArithmetic::toString([[maybe_unused]] bool braces) {
  return std::to_string(m_value);
}

std::string BinaryArithmeticOperation::toString(bool braces) {
  std::string output{m_left->toString(braces) + static_cast<char>(m_token.m_token) +
                     m_right->toString(braces)};
  if (braces) {
    return {"(" + output + ")"};
  } else {
    return output;
  }
}

std::string UnaryArithmeticOperation::toString(bool braces) {
  std::string out{" "};
  out += static_cast<char>(m_token.m_token);
  out += m_input->toString(braces);
  if (braces) {
    return {"(" + out + ")"};
  } else {
    return out;
  }
}

std::string FunctionArithmetic::toString(bool braces) {
  switch (m_token.m_token) {
  case Token::Sin: {
    return {" sin(" + m_input->toString(braces) + ")"};
  }
  case Token::Cos: {
    return {" cos(" + m_input->toString(braces) + ")"};
  }
  case Token::Tan: {
    return {" tan(" + m_input->toString(braces) + ")"};
  }
  case Token::Atan: {
    return {" atan(" + m_input->toString(braces) + ")"};
  }
  case Token::Acos: {
    return {" acos(" + m_input->toString(braces) + ")"};
  }
  case Token::Asin: {
    return {" asin(" + m_input->toString(braces) + ")"};
  }
  case Token::Log: {
    return {" log(" + m_input->toString(braces) + ")"};
  }
  case Token::Sqrt: {
    return {" sqrt(" + m_input->toString(braces) + ")"};
  }
  case Token::Int: {
    return {" Int(" + m_input->toString(braces) + ")"};
  }
  default: {
    assert(false);
    return {""};
  }
  }
}

std::string AtomicBoolean::toString([[maybe_unused]] bool braces) {
  if (m_value) {
    return {" true"};
  } else {
    return {" false "};
  }
}

std::string BinaryBooleanOperation::toString(bool braces) {
  std::string output;
  switch (m_token.m_token) {

  case Token::And: {
    output = {m_left->toString(braces) + " and " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case Token::Or: {
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

std::string UnaryBooleanOperation::toString(bool braces) {
  std::string output{m_input->toString(braces)};
  // currently the only unary boolean token
  if (braces) {
    return {" not (" + output + ")"};
  } else {
    return {" not " + output};
  }
}

std::string Comparision::toString(bool braces) {
  std::string output;
  switch (m_token.m_token) {
  case Token::Greater_than: {
    output = {m_left->toString(braces) + " greater_than " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case Token::Less_than: {
    output = {m_left->toString(braces) + " less_than " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case Token::Equal_to: {
    output = {m_left->toString(braces) + " equal_to " + m_right->toString(braces)};
    if (braces) {
      return {"(" + output + ")"};
    } else {
      return output;
    }
  }
  case Token::Not_equal_to: {
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

std::string Assignment::toString(bool braces) {
  return {"var " + m_var + " = " + m_value->toString(braces) + ";\n"};
}

std::string Print::toString(bool braces) { return {m_value->toString(braces) + ";\n"}; }

std::string Variable::toString([[maybe_unused]] bool braces) { return {" " + m_var + " "}; }
