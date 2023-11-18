#include "Lexer.hpp"
#include "Errors.hpp"
#include "tokens.hpp"
#include <cctype>

Lexer::Lexer(std::istringstream &&ss)
    : m_iss(std::move(ss)), m_buffer(""), m_postion(0), m_line(0), m_cur_token(getToken()) {}

const TokenData Lexer::getCurrentToken() const { return m_cur_token; }

void Lexer::advance() {
  if (m_tokenBuffer.empty()) {
    m_cur_token = getToken();
  } else {
    m_cur_token = m_tokenBuffer.top();
    m_tokenBuffer.pop();
  }
};

const TokenData Lexer::getToken() {
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
    return {Token::EOF_sym, m_postion, m_line, m_buffer};
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

    Token out{};

    if (m_buffer == "sin") {
      out = Token::Sin;
    } else if (m_buffer == "cos") {
      out = Token::Cos;
    } else if (m_buffer == "tan") {
      out = Token::Tan;
    } else if (m_buffer == "asin") {
      out = Token::Asin;
    } else if (m_buffer == "acos") {
      out = Token::Acos;
    } else if (m_buffer == "atan") {
      out = Token::Atan;
    } else if (m_buffer == "log") {
      out = Token::Log;
    } else if (m_buffer == "sqrt") {
      out = Token::Sqrt;
    } else if (m_buffer == "Int") {
      out = Token::Int;
    } else if (m_buffer == "equal_to") {
      out = Token::Equal_to;
    } else if (m_buffer == "not_equal_to") {
      out = Token::Not_equal_to;
    } else if (m_buffer == "less_than") {
      out = Token::Less_than;
    } else if (m_buffer == "greater_than") {
      out = Token::Greater_than;
    } else if (m_buffer == "true") {
      out = Token::True;
    } else if (m_buffer == "false") {
      out = Token::False;
    } else if (m_buffer == "and") {
      out = Token::And;
    } else if (m_buffer == "or") {
      out = Token::Or;
    } else if (m_buffer == "not") {
      out = Token::Not;
    } else if (m_buffer == "var") {
      out = Token::Var;
    } else {
      // must be an identifier
      out = Token::Id;
    }

    return {out, m_postion, m_line, m_buffer};
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
    return {Token::Number, m_postion, m_line, m_buffer};
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
  case ';':
    return {Token(c), m_postion, m_line, m_buffer};
  }

  std::string out{"Line: "};
  out.append(std::to_string(m_line));
  out.append(" Postion: ");
  out.append(std::to_string(m_postion));

  throw LexicalError{m_buffer, out};
}

void Lexer::pushBackToken(TokenData token) { m_tokenBuffer.push(token); }
