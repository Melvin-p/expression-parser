#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include <cstddef>
#include <sstream>
#include <string>

class Lexer {
private:
  std::istringstream m_iss;
  std::string m_buffer;
  Token m_cur_token;
  std::size_t m_postion{};
  std::size_t m_line{};

public:
  explicit Lexer(std::istringstream &&ss);
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  Lexer(Lexer &&) = delete;
  Lexer &operator=(Lexer &&) = delete;

  Token getCurrentToken() const;
  std::string getCurrentTokenText() const;
  /*move to the next token*/
  void advance();
  std::string getLocation() const;

private:
  /*get the next token in the stream*/
  Token getToken();
};

#endif