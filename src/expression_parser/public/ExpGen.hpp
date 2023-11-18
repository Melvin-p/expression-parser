#ifndef EXP_GEN_HPP
#define EXP_GEN_HPP

#include "Node.hpp"
#include "Types.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class ExpGen {
private:
  std::unordered_map<std::string, DataTypes> m_symbol_table{};
  std::vector<std::string> m_bools{};
  std::vector<std::string> m_doubles{};
  struct Data;
  std::unique_ptr<Data> m_data;

public:
  ExpGen();
  ~ExpGen();
  std::unique_ptr<Arithmetic> genArithmetic(const double prob = 1, bool unary = true);
  std::unique_ptr<Boolean> genBoolean(const double prob = 1, bool unary = true);
  std::unique_ptr<Program> getStatements(std::size_t count = 10);

private:
  std::string getNewVariable();
};

#endif
