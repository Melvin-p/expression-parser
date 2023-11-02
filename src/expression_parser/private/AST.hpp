#ifndef AST_HPP
#define AST_HPP

#include "Node.hpp"
#include "tokens.hpp"
#include <memory>
#include <string>

class Variable : public Arithmetic, public Boolean {
private:
  TokenData m_token;

public:
  Variable(TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual double evalGetDouble(const SymbolTable &symbol_table) const override;
  virtual bool evalGetBool(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class AtomicArithmetic : public Arithmetic {
private:
  TokenData m_token;

public:
  AtomicArithmetic(TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual double evalGetDouble(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class ParenthesesArithmetic : public Arithmetic {
private:
  TokenData m_token;
  std::unique_ptr<Arithmetic> m_input;

public:
  ParenthesesArithmetic(std::unique_ptr<Expression> &&input, TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual double evalGetDouble(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class BinaryArithmeticOperation : public Arithmetic {
private:
  std::unique_ptr<Arithmetic> m_left;
  TokenData m_token;
  std::unique_ptr<Arithmetic> m_right;

public:
  BinaryArithmeticOperation(std::unique_ptr<Expression> &&left, TokenData &&token,
                            std::unique_ptr<Expression> &&right);

  virtual std::string toString(const bool braces) const override;
  virtual double evalGetDouble(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class UnaryArithmeticOperation : public Arithmetic {
private:
  std::unique_ptr<Arithmetic> m_input;
  TokenData m_token;

public:
  UnaryArithmeticOperation(std::unique_ptr<Expression> &&input, TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual double evalGetDouble(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class FunctionArithmetic : public Arithmetic {
private:
  std::unique_ptr<Arithmetic> m_input;
  TokenData m_token;

public:
  FunctionArithmetic(std::unique_ptr<Expression> &&input, TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual double evalGetDouble(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class AtomicBoolean : public Boolean {
private:
  TokenData m_token;

public:
  AtomicBoolean(TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual bool evalGetBool(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class ParenthesesBoolean : public Boolean {
private:
  TokenData m_token;
  std::unique_ptr<Boolean> m_input;

public:
  ParenthesesBoolean(std::unique_ptr<Expression> &&input, TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual bool evalGetBool(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class BinaryBooleanOperation : public Boolean {
private:
  std::unique_ptr<Boolean> m_left;
  TokenData m_token;
  std::unique_ptr<Boolean> m_right;

public:
  BinaryBooleanOperation(std::unique_ptr<Expression> &&left, TokenData &&token,
                         std::unique_ptr<Expression> &&right);
  virtual std::string toString(const bool braces) const override;
  virtual bool evalGetBool(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class Comparision : public Boolean {
private:
  std::unique_ptr<Arithmetic> m_left;
  TokenData m_token;
  std::unique_ptr<Arithmetic> m_right;

public:
  Comparision(std::unique_ptr<Expression> &&left, TokenData &&token,
              std::unique_ptr<Expression> &&right);
  virtual std::string toString(const bool braces) const override;
  virtual bool evalGetBool(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class UnaryBooleanOperation : public Boolean {
private:
  std::unique_ptr<Boolean> m_input;
  TokenData m_token;

public:
  UnaryBooleanOperation(std::unique_ptr<Expression> &&input, TokenData &&token);
  virtual std::string toString(const bool braces) const override;
  virtual bool evalGetBool(const SymbolTable &symbol_table) const override;
  virtual var eval(const SymbolTable &symbol_table) const override;
};

class Print : public Statement {
private:
  std::unique_ptr<Expression> m_value;

public:
  Print(std::unique_ptr<Expression> &&value) : m_value(std::move(value)) {}
  virtual std::string toString(const bool braces) const override;
  virtual std::string evalGetString(SymbolTable &symbol_table) const override;
};

class Assignment : public Statement {
private:
  TokenData m_token;
  std::unique_ptr<Expression> m_value;
  bool m_create_var;

public:
  Assignment(std::unique_ptr<Expression> &&value, TokenData &&token, bool create_var);
  virtual std::string toString(const bool braces) const override;
  virtual std::string evalGetString(SymbolTable &symbol_table) const override;
};

#endif