#include "Errors.hpp"
#include <string>

LexicalError::LexicalError(std::string message) { m_message.append(message); };

const char *LexicalError::what() const noexcept { return m_message.c_str(); };

SyntaxError::SyntaxError(std::string message) { m_message.append(message); }

const char *SyntaxError::what() const noexcept { return m_message.c_str(); }