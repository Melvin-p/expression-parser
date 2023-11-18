#include "Errors.hpp"

LexicalError::LexicalError(std::string message, std::string location) {
  m_message.append(message);
  m_message.append(" ");
  m_message.append(location);
};

const char *LexicalError::what() const noexcept { return m_message.c_str(); };

SyntaxError::SyntaxError(std::string message, std::string location) {
  m_message.append(message);
  m_message.append(" ");
  m_message.append(location);
}

const char *SyntaxError::what() const noexcept { return m_message.c_str(); }

RuntimeError::RuntimeError(std::string message, std::string location) {
  m_message.append(message);
  m_message.append(" ");
  m_message.append(location);
}

const char *RuntimeError::what() const noexcept { return m_message.c_str(); }
