#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <cstddef>
#include <string>

enum class Token {
  Id,
  Number,
  Sin,
  Cos,
  Tan,
  Asin,
  Acos,
  Atan,
  Log,
  Sqrt,
  Int,
  Equal_to,
  Not_equal_to,
  Less_than,
  Greater_than,
  True,
  False,
  And,
  Or,
  Not,
  Var,
  Print,
  Semicolon = ';',
  Assign = '=',
  Plus = '+',
  Minus = '-',
  Mul = '*',
  Div = '/',
  Mod = '%',
  Pow = '^',
  Lp = '(',
  Rp = ')',
  EOF_sym = -1
};

class TokenData {
public:
  Token m_token;

private:
  std::size_t m_postion;
  std::size_t m_line;
  std::string m_text;

public:
  TokenData(Token token, std::size_t postion, std::size_t line, std::string text)
      : m_token(token), m_postion(postion), m_line(line), m_text(text) {}

  TokenData(const TokenData &token_data) = default;
  TokenData(TokenData &&token_data) = default;
  TokenData &operator=(const TokenData &token_data) = default;

  const std::string getLocation() const {
    std::string out{"Line: "};
    out.append(std::to_string(m_line));
    out.append(" Postion: ");
    out.append(std::to_string(m_postion));
    return out;
  }

  const std::string getText() { return m_text; }
};

#endif