#include "Lexer.hpp"
#include "Errors.hpp"
#include "tokens.hpp"
#include <cctype>
#include <string>

Lexer::Lexer(std::istringstream &&ss)
    : m_iss(std::move(ss)), m_buffer(""), m_cur_token(getToken()) {}

std::string Lexer::getCurrentTokenText() const { return m_buffer; }

Token Lexer::getCurrentToken() const { return m_cur_token; }

void Lexer::advance() { m_cur_token = getToken(); };

Token Lexer::getToken() {
  m_buffer.clear();
  int c = m_iss.get();
  if (c == '\n') {
    ++m_line;
    m_postion = 0;
  } else {
    ++m_postion;
  }
  while (std::isspace(c)) {
    c = m_iss.get();
    if (c == '\n') {
      ++m_line;
      m_postion = 0;
    } else {
      ++m_postion;
    }
  }
  if (!m_iss) {
    return Token::EOF_sym;
  }
  if (std::isalpha(c)) {
    m_buffer.push_back(static_cast<char>(c));
    c = m_iss.get();
    ++m_postion;
    while (std::isalnum(c) || c == '_') {
      m_buffer.push_back(static_cast<char>(c));
      c = m_iss.get();
      ++m_postion;
    }
    m_iss.putback(static_cast<char>(c));
    --m_postion;

    if (m_buffer == "sin") {
      return Token::Sin;
    }
    if (m_buffer == "cos") {
      return Token::Cos;
    }
    if (m_buffer == "tan") {
      return Token::Tan;
    }
    if (m_buffer == "asin") {
      return Token::Asin;
    }
    if (m_buffer == "acos") {
      return Token::Acos;
    }
    if (m_buffer == "atan") {
      return Token::Atan;
    }
    if (m_buffer == "log") {
      return Token::Log;
    }
    if (m_buffer == "sqrt") {
      return Token::Sqrt;
    }
    if (m_buffer == "Int") {
      return Token::Int;
    }
    if (m_buffer == "equal_to") {
      return Token::Equal_to;
    }
    if (m_buffer == "not_equal_to") {
      return Token::Not_equal_to;
    }
    if (m_buffer == "less_than") {
      return Token::Less_than;
    }
    if (m_buffer == "greater_than") {
      return Token::Greater_than;
    }
    if (m_buffer == "true") {
      return Token::True;
    }
    if (m_buffer == "false") {
      return Token::False;
    }
    if (m_buffer == "and") {
      return Token::And;
    }
    if (m_buffer == "or") {
      return Token::Or;
    }
    if (m_buffer == "not") {
      return Token::Not;
    }
    if (m_buffer == "var") {
      return Token::Var;
    }
    if (m_buffer == "Print") {
      return Token::Print;
    }
    // must be an identifier
    return Token::Id;
  }
  // number or decimal
  if (std::isdigit(c)) {
    m_buffer.push_back(static_cast<char>(c));
    c = m_iss.get();
    ++m_postion;
    while (std::isdigit(c)) {
      m_buffer.push_back(static_cast<char>(c));
      c = m_iss.get();
      ++m_postion;
    }
    // decimal point found
    if (c == '.') {
      m_buffer.push_back(static_cast<char>(c));
      c = m_iss.get();
      ++m_postion;
      while (std::isdigit(c)) {
        m_buffer.push_back(static_cast<char>(c));
        c = m_iss.get();
        ++m_postion;
      }
    }
    m_iss.putback(static_cast<char>(c));
    --m_postion;
    return Token::Number;
  }
  m_buffer.push_back(static_cast<char>(c));
  switch (c) {
  case '=':
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '^':
  case '(':
  case ')':
    return Token(c);
  }

  throw LexicalError{m_buffer, getLocation()};
}

std::string Lexer::getLocation() const {
  std::string out{"Line: "};
  out.append(std::to_string(m_line));
  out.append(" Postion: ");
  out.append(std::to_string(m_postion));
  return out;
}
