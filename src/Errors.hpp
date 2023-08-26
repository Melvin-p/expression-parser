#ifndef ERRORS_EP_HPP
#define ERRORS_EP_HPP

#include <exception>
#include <string>
#include <string_view>

class LexicalError : public std::exception {
private:
  std::string m_message{"Bad things happened during lexing\n"};

public:
  LexicalError(std::string message);

  virtual const char *what() const noexcept;
};

class SyntaxError : public std::exception {
private:
  std::string m_message{"Syntax Error Occurred\n"};

public:
  SyntaxError(std::string message);
  virtual const char *what() const noexcept;
};

#endif