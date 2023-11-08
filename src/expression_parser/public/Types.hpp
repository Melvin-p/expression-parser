#ifndef TYPES_INTERNAL_HPP
#define TYPES_INTERNAL_HPP

#include <string>
#include <unordered_map>
#include <variant>

enum class DataTypes {
  bool_,
  double_,
};

using var = std::variant<bool, double>;
using SymbolTable = std::unordered_map<std::string, var>;

#endif