#include "ActionTokens.hpp"

ActionTokenData::ActionTokenData(TokenData &lexer_token, ActionTokens token)
    : m_token(token), m_postion(lexer_token.getPostion()), m_line(lexer_token.getLine()),
      m_text(lexer_token.getText()) {}

ActionTokenData::ActionTokenData(ActionTokens token) : m_token(token) {}

ActionTokens ActionTokenData::getToken() const { return m_token; }

const std::string ActionTokenData::getLocation() const {
  std::string out{"Line: "};
  out.append(std::to_string(m_line));
  out.append(" Postion: ");
  out.append(std::to_string(m_postion));
  return out;
}

const std::string ActionTokenData::getText() const { return m_text; }

const std::string ActionTokenData::getOperation() const {
  switch (m_token) {
  case ActionTokens::sin:
    return "sine";
  case ActionTokens::cos:
    return "cosine";
  case ActionTokens::tan:
    return "tan";
  case ActionTokens::Asin:
    return "arc sine";
  case ActionTokens::Acos:
    return "arc cosine";
  case ActionTokens::Atan:
    return "arc tan";
  case ActionTokens::Log:
    return "log";
  case ActionTokens::Sqrt:
    return "square root";
  case ActionTokens::Int:
    return "Int";
  case ActionTokens::Equal_to:
    return "equal to";
  case ActionTokens::Not_equal_to:
    return "not equal to";
  case ActionTokens::Less_than:
    return "less than";
  case ActionTokens::Greater_than:
    return "greater than";
  case ActionTokens::And:
    return "and";
  case ActionTokens::Or:
    return "or";
  case ActionTokens::Not:
    return "not";
  case ActionTokens::Assignment:
    return "assignment";
  case ActionTokens::Addition:
    return "addition";
  case ActionTokens::Subtraction:
    return "subtraction";
  case ActionTokens::Division:
    return "division";
  case ActionTokens::Multiplication:
    return "multiplication";
  case ActionTokens::Modulo:
    return "modulo";
  case ActionTokens::Power:
    return "power";
  case ActionTokens::negative:
    return "negative";
  case ActionTokens::positive:
    return "positive";
  }
}
