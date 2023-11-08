#include "tokens.hpp"
#include <cstddef>
#include <string>

TokenData::TokenData(Token token, std::size_t postion, std::size_t line, std::string text)
    : m_token(token), m_postion(postion), m_line(line), m_text(text) {}

const std::string TokenData::getLocation() const {
  std::string out{"Line: "};
  out.append(std::to_string(m_line));
  out.append(" Postion: ");
  out.append(std::to_string(m_postion));
  return out;
}

Token TokenData::getToken() const { return m_token; }

const std::string TokenData::getText() const { return m_text; }

std::size_t TokenData::getPostion() { return m_postion; }

std::size_t TokenData::getLine() { return m_line; }
