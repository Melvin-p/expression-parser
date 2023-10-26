#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class Node {
private:
public:
  using var = std::variant<bool, double>;
  using SymbolTable = std::unordered_map<std::string, var>;
  virtual std::string toString(const bool braces) const = 0;
  virtual ~Node() = default;
};

class Expression : public Node {};

class Arithmetic : virtual public Expression {
public:
  virtual double evalGetDouble(const SymbolTable &symbol_table) const = 0;
};

class Boolean : virtual public Expression {
public:
  virtual bool evalGetBool(const SymbolTable &symbol_table) const = 0;
};

class Statement : public Node {
public:
  virtual std::string evalGetString(SymbolTable &symbol_table) const = 0;
};

class Program : Node {
private:
  std::vector<std::unique_ptr<Statement>> m_statements{};

public:
  Program() = default;
  virtual std::string toString(const bool braces) const override;
  void append(std::unique_ptr<Statement> &&s);
  std::string eval(SymbolTable &symbol_table);
};

#endif