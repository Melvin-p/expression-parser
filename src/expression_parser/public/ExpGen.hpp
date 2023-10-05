#ifndef EXP_GEN_HPP
#define EXP_GEN_HPP

#include "Node.hpp"
#include "Random.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class ExpGen {
private:
  using var = std::variant<bool, double>;
  /*
  true if double
  false if bool
  */
  std::unordered_map<std::string, bool> m_symbol_table{};
  std::vector<std::string> m_bools{};
  std::vector<std::string> m_doubles{};
  Random m_random{};

public:
  ExpGen() = default;
  std::unique_ptr<Arithmetic> genArithmetic(const double prob = 1, bool unary = true);
  std::unique_ptr<Boolean> genBoolean(const double prob = 1, bool unary = true);
  std::unique_ptr<Program> getStatements(std::size_t count = 10);

private:
  std::string getNewVariable();
};

#endif
