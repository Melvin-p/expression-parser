#ifndef AST_TOKEN_HPP
#define AST_TOKEN_HPP

#include "tokens.hpp"
#include <cstddef>

enum class ActionTokens {
  sin,
  cos,
  tan,
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
  And,
  Or,
  Not,
  Assignment,
  Addition,
  Subtraction,
  Division,
  Multiplication,
  Modulo,
  Power,
  negative,
  positive
};

class ActionTokenData {
public:
  ActionTokens m_token;

private:
  std::size_t m_postion{0};
  std::size_t m_line{0};
  std::string m_text{""};

public:
  ActionTokenData(TokenData &lexer_token, ActionTokens token);
  ActionTokenData(ActionTokens token);
  ActionTokenData(const ActionTokenData &token_data) = default;
  ActionTokenData(ActionTokenData &&token_data) = default;
  ActionTokenData &operator=(const ActionTokenData &token_data) = default;

  ActionTokens getToken() const;
  const std::string getLocation() const;
  const std::string getText() const;
  const std::string getOperation() const;
  const std::string getOperator() const;
};

#endif