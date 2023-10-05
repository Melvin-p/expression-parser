#ifndef AST_HPP
#define AST_HPP

#include "Node.hpp"
#include "tokens.hpp"
#include <memory>
#include <string>
#include <vector>

class Variable : public Arithmetic, public Boolean {
private:
  std::string m_var;

public:
  Variable(std::string var) : m_var(var) {}
  virtual std::string toString(bool braces) override;
};

class AtomicArithmetic : public Arithmetic {
private:
  double m_value{};

public:
  AtomicArithmetic(double value) : m_value(value){};
  virtual std::string toString(bool braces) override;
};

class ParenthesesArithmetic : public Arithmetic {
private:
  std::unique_ptr<Arithmetic> m_input;

public:
  ParenthesesArithmetic(std::unique_ptr<Arithmetic> &&input) : m_input(std::move(input)) {}
  virtual std::string toString(bool braces) override;
};

class BinaryArithmeticOperation : public Arithmetic {
private:
  std::unique_ptr<Arithmetic> m_left;
  TokenData m_token;
  std::unique_ptr<Arithmetic> m_right;

public:
  BinaryArithmeticOperation(std::unique_ptr<Arithmetic> &&left, TokenData token,
                            std::unique_ptr<Arithmetic> &&right)
      : m_left(std::move(left)), m_token(token), m_right(std::move(right)) {}

  virtual std::string toString(bool braces) override;
};

class UnaryArithmeticOperation : public Arithmetic {
private:
  std::unique_ptr<Arithmetic> m_input;
  TokenData m_token;

public:
  UnaryArithmeticOperation(std::unique_ptr<Arithmetic> &&input, TokenData token)
      : m_input(std::move(input)), m_token(token) {}
  virtual std::string toString(bool braces) override;
};

class FunctionArithmetic : public Arithmetic {
private:
  std::unique_ptr<Arithmetic> m_input;
  TokenData m_token;

public:
  FunctionArithmetic(std::unique_ptr<Arithmetic> &&input, TokenData token)
      : m_input(std::move(input)), m_token(token) {}
  virtual std::string toString(bool braces) override;
};

class AtomicBoolean : public Boolean {
private:
  bool m_value{};

public:
  AtomicBoolean(bool value) : m_value(value){};
  virtual std::string toString(bool braces) override;
};

class ParenthesesBoolean : public Boolean {
private:
  std::unique_ptr<Boolean> m_input;

public:
  ParenthesesBoolean(std::unique_ptr<Boolean> &&input) : m_input(std::move(input)) {}
  virtual std::string toString(bool braces) override;
};

class BinaryBooleanOperation : public Boolean {
private:
  std::unique_ptr<Boolean> m_left;
  TokenData m_token;
  std::unique_ptr<Boolean> m_right;

public:
  BinaryBooleanOperation(std::unique_ptr<Boolean> &&left, TokenData token,
                         std::unique_ptr<Boolean> &&right)
      : m_left(std::move(left)), m_token(token), m_right(std::move(right)){};
  virtual std::string toString(bool braces) override;
};

class Comparision : public Boolean {
private:
  std::unique_ptr<Arithmetic> m_left;
  TokenData m_token;
  std::unique_ptr<Arithmetic> m_right;

public:
  Comparision(std::unique_ptr<Arithmetic> &&left, TokenData token,
              std::unique_ptr<Arithmetic> &&right)
      : m_left(std::move(left)), m_token(token), m_right(std::move(right)){};
  virtual std::string toString(bool braces) override;
};

class UnaryBooleanOperation : public Boolean {
private:
  std::unique_ptr<Boolean> m_input;
  [[maybe_unused]] TokenData m_token;

public:
  UnaryBooleanOperation(std::unique_ptr<Boolean> &&input, TokenData token)
      : m_input(std::move(input)), m_token(token) {}
  virtual std::string toString(bool braces) override;
};

class Print : public Statement {
private:
  std::unique_ptr<Expression> m_value;

public:
  Print(std::unique_ptr<Expression> &&value) : m_value(std::move(value)) {}
  virtual std::string toString(bool braces) override;
};

class Assignment : public Statement {
private:
  std::string m_var;
  std::unique_ptr<Expression> m_value;

public:
  Assignment(std::unique_ptr<Expression> &&value, std::string var)
      : m_var(var), m_value(std::move(value)) {}
  virtual std::string toString(bool braces) override;
};

#endif