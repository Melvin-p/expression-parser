#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <cstddef>
#include <string>

/**
 * @brief
 * Holds tokens for tokenizer and AST
 */
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

/**
 * @brief
 * holds token and other information associated with the token
 */
class TokenData {
public:
  Token m_token;

private:
  std::size_t m_postion;
  std::size_t m_line;
  std::string m_text;
  std::string m_operation;

public:
  TokenData(Token token, std::size_t postion = 0, std::size_t line = 0, std::string text = "");
  TokenData(const TokenData &token_data) = default;
  TokenData(TokenData &&token_data) = default;

  TokenData &operator=(const TokenData &token_data) = default;

  Token getToken() const;
  const std::string getLocation() const;
  const std::string getText() const;
  const std::string getOperation() const;

private:
  /**
   * @brief returns action performed by token
   * for example `Token::Plus` becomes addition
   *
   * @return const std::string
   */
  const std::string tokenToAction() const;
};

#endif