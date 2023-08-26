#ifndef ERRORS_EP_HPP
#define ERRORS_EP_HPP

#include <exception>
#include <string>
#include <string_view>

class LexicalError : public std::exception {
private:
  std::string m_message{"Lexing Error\n"};

public:
  LexicalError(std::string message, std::string location);

  virtual const char *what() const noexcept;
};

class SyntaxError : public std::exception {
private:
  std::string m_message{"Syntax Error Occurred\n"};

public:
  SyntaxError(std::string message, std::string location);

  virtual const char *what() const noexcept;
};

class RuntimeError : public std::exception {
private:
  std::string m_message{"Runtime Error"};

public:
  RuntimeError(std::string message, std::string location);
  virtual const char *what() const noexcept;
};

#endif