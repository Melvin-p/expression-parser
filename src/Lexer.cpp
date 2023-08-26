#include "Lexer.hpp"
#include "Errors.hpp"
#include <cctype>

Lexer::Lexer(std::istringstream &&ss)
    : m_iss(std::move(ss)), m_buffer(""), m_cur_token(getToken()) {}

std::string Lexer::getCurrentTokenText() const { return m_buffer; }

Token Lexer::getCurrentToken() const { return m_cur_token; }

void Lexer::advance() { m_cur_token = getToken(); };

Token Lexer::getToken() {
  m_buffer.clear();
  int c = m_iss.get();
  while (std::isspace(c)) {
    c = m_iss.get();
  }
  if (!m_iss) {
    return Token::EOF_sym;
  }
  if (std::isalpha(c)) {
    m_buffer.push_back(static_cast<char>(c));
    c = m_iss.get();
    while (std::isalnum(c)) {
      m_buffer.push_back(static_cast<char>(c));
      c = m_iss.get();
    }
    m_iss.putback(static_cast<char>(c));

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
    if (m_buffer == "int") {
      return Token::Int;
    }
    // must be an identifier
    return Token::Id;
  }
  // number or decimal
  if (std::isdigit(c)) {
    m_buffer.push_back(static_cast<char>(c));
    c = m_iss.get();
    while (std::isdigit(c)) {
      m_buffer.push_back(static_cast<char>(c));
      c = m_iss.get();
    }
    // decimal point found
    if (c == '.') {
      m_buffer.push_back(static_cast<char>(c));
      c = m_iss.get();
      while (std::isdigit(c)) {
        m_buffer.push_back(static_cast<char>(c));
        c = m_iss.get();
      }
    }
    m_iss.putback(static_cast<char>(c));
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

  throw LexicalError{m_buffer};
}
