#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>
#include <variant>
#include <vector>

class Node {
private:
public:
  using var = std::variant<bool, double>;
  virtual std::string toString(bool braces) = 0;
  virtual ~Node() = default;
};

class Expression : public Node {
  // virtual var eval();
};

class Arithmetic : virtual public Expression {};
class Boolean : virtual public Expression {};

class Statement : public Node {
  // std::string eval();
};

class Program : Node {
private:
  std::vector<std::unique_ptr<Statement>> m_statements{};

public:
  Program() = default;
  virtual std::string toString(bool braces) override;
  void append(std::unique_ptr<Statement> &&s);
};

#endif