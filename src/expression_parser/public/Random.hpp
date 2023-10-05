#ifndef RANDOM_UTILITY_HPP
#define RANDOM_UTILITY_HPP

#include "tokens.hpp"
#include <cstddef>
#include <random>
#include <string>

class Random {
private:
  std::random_device m_rd{};
  std::seed_seq m_ss{m_rd(), m_rd(), m_rd(), m_rd(), m_rd(), m_rd(), m_rd(), m_rd()};
  std::minstd_rand m_mt{m_ss};

public:
  Random() = default;
  Token getArithBinaryOp();
  Token getArithUnaryOp();
  Token getFunction();
  Token getBinaryOp();
  Token getCompOp();
  double getReal0to1();
  double getReal0to9();
  int getInteger1to12();
  int get0or1();
  bool getBool();
  std::string getString(std::size_t n = 5);
  std::size_t getInteger0toN(std::size_t n);
};

#endif