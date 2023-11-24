#ifndef NODE_HPP
#define NODE_HPP

#include "Types.hpp"
#include <memory>
#include <string>
#include <vector>

class Node {
private:
public:
  Node() = default;
  Node(const Node &other) = delete;
  Node &operator=(const Node &t) = delete;
  virtual std::string toString(const bool braces) const = 0;
  virtual ~Node() = default;
};

class Expression : public Node {
public:
  Expression() = default;
  Expression(const Expression &other) = delete;
  Expression &operator=(const Expression &t) = delete;
  /**
   * @brief calls evalGetDouble or evalGetBool as required
   *
   * @param symbol_table
   * @return var
   */
  virtual var eval(const SymbolTable &symbol_table) const = 0;
  /**
   * @brief Get the data type of the expression node
   *
   * @param symbol_table
   * @return DataTypes
   */
  virtual DataTypes getDataType(const SymbolTable &symbol_table) const = 0;
};

class Arithmetic : virtual public Expression {
public:
  Arithmetic() = default;
  Arithmetic(const Arithmetic &other) = delete;
  Arithmetic &operator=(const Arithmetic &t) = delete;
  virtual double evalGetDouble(const SymbolTable &symbol_table) const = 0;
  virtual DataTypes getDataType(const SymbolTable &symbol_table) const override;
};

class Boolean : virtual public Expression {
public:
  Boolean() = default;
  Boolean(const Boolean &other) = delete;
  Boolean &operator=(const Boolean &t) = delete;
  virtual bool evalGetBool(const SymbolTable &symbol_table) const = 0;
  virtual DataTypes getDataType(const SymbolTable &symbol_table) const override;
};

class Statement : public Node {
public:
  Statement() = default;
  Statement(const Statement &other) = delete;
  Statement &operator=(const Statement &t) = delete;
  virtual std::string evalGetString(SymbolTable &symbol_table) const = 0;
};

class Program : Node {
private:
  std::vector<std::unique_ptr<Statement>> m_statements{};

public:
  Program() = default;
  Program(const Program &other) = delete;
  Program &operator=(const Program &t) = delete;
  virtual std::string toString(const bool braces) const override;
  void append(std::unique_ptr<Statement> &&s);
  std::string eval(SymbolTable &symbol_table) const;
};

#endif