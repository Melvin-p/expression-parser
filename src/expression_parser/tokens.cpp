#include "tokens.hpp"
#include <string>

TokenData::TokenData(Token token, std::size_t postion, std::size_t line, std::string text)
    : m_token(token), m_postion(postion), m_line(line), m_text(text), m_operation(tokenToAction()) {
}

const std::string TokenData::getLocation() const {
  std::string out{"Line: "};
  out.append(std::to_string(m_line));
  out.append(" Postion: ");
  out.append(std::to_string(m_postion));
  return out;
}

Token TokenData::getToken() const { return m_token; }

const std::string TokenData::getText() const { return m_text; }

const std::string TokenData::tokenToAction() const {
  switch (m_token) {
  case Token::Id: {
    return "";
  }
  case Token::Number: {
    return "";
  }
  case Token::Sin: {
    return "sin";
  }
  case Token::Cos: {
    return "cos";
  }
  case Token::Tan: {
    return "tan";
  }
  case Token::Asin: {
    return "asin";
  }
  case Token::Acos: {
    return "acos";
  }
  case Token::Atan: {
    return "atan";
  }
  case Token::Log: {
    return "log";
  }
  case Token::Sqrt: {
    return "sqrt";
  }
  case Token::Int: {
    return "Int";
  }
  case Token::Equal_to: {
    return "equal";
  }
  case Token::Not_equal_to: {
    return "not equal";
  }
  case Token::Less_than: {
    return "less than";
  }
  case Token::Greater_than: {
    return "greater than";
  }
  case Token::True: {
    return "";
  }
  case Token::False: {
    return "";
  }
  case Token::And: {
    return "and";
  }
  case Token::Or: {
    return "or";
  }
  case Token::Not: {
    return "not";
  }
  case Token::Var: {
    return "";
  }
  case Token::Semicolon: {
    return "";
  }
  case Token::Assign: {
    return "assignment";
  }
  case Token::Plus: {
    return "addition";
  }
  case Token::Minus: {
    return "subtraction";
  }
  case Token::Mul: {
    return "multiplication";
  }
  case Token::Div: {
    return "division";
  }
  case Token::Mod: {
    return "modulo";
  }
  case Token::Pow: {
    return "power";
  }
  case Token::Lp: {
    return "";
  }
  case Token::Rp: {
    return "";
  }
  case Token::EOF_sym: {
    return "";
  }
  }
}

const std::string TokenData::getOperation() const { return m_operation; }