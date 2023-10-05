#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include <cstddef>
#include <sstream>
#include <stack>
#include <string>

class Lexer {
private:
  std::istringstream m_iss;
  std::string m_buffer;
  std::size_t m_postion{};
  std::size_t m_line{};
  TokenData m_cur_token;
  std::stack<TokenData> m_tokenBuffer{};

public:
  explicit Lexer(std::istringstream &&ss);
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  Lexer(Lexer &&) = delete;
  Lexer &operator=(Lexer &&) = delete;

  const TokenData getCurrentToken() const;

  /*move to the next token*/
  void advance();

  void pushBackToken(TokenData token);

private:
  /*get the next token in the stream*/
  const TokenData getToken();
};

#endif