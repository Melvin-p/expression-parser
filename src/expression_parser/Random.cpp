#include "Random.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <array>
#include <cstddef>

Token Random::getArithBinaryOp() {
  const constexpr std::array<const Token, 6> operators{Token::Plus, Token::Minus, Token::Mul,
                                                       Token::Mod,  Token::Div,   Token::Pow};

  std::uniform_int_distribution<std::size_t> operator_choice{0, operators.size() - 1};
  return operators[operator_choice(m_mt)];
}

Token Random::getArithUnaryOp() {
  const constexpr std::array<const Token, 2> operators{Token::Plus, Token::Minus};
  std::uniform_int_distribution<std::size_t> operator_choice{0, operators.size() - 1};
  return operators[operator_choice(m_mt)];
}

Token Random::getFunction() {
  const constexpr std::array<const Token, 9> functions{Token::Sin,  Token::Cos,  Token::Tan,
                                                       Token::Asin, Token::Atan, Token::Acos,
                                                       Token::Log,  Token::Sqrt, Token::Int};

  std::uniform_int_distribution<std::size_t> operator_choice{0, functions.size() - 1};
  return functions[operator_choice(m_mt)];
}

Token Random::getBinaryOp() {
  const constexpr std::array<const Token, 2> operators{Token::And, Token::Or};
  std::uniform_int_distribution<std::size_t> operator_choice{0, operators.size() - 1};
  return operators[operator_choice(m_mt)];
}

Token Random::getCompOp() {
  const constexpr std::array<const Token, 4> operators{Token::Greater_than, Token::Less_than,
                                                       Token::Equal_to, Token::Not_equal_to};
  std::uniform_int_distribution<std::size_t> operator_choice{0, operators.size() - 1};
  return operators[operator_choice(m_mt)];
}

double Random::getReal0to1() {
  std::uniform_real_distribution<double> dist_real_0_1{0, 1};
  return dist_real_0_1(m_mt);
}

double Random::getReal0to9() {
  std::uniform_real_distribution<double> dist_real_0_9{0, 9};
  return dist_real_0_9(m_mt);
}

int Random::getInteger1to12() {
  std::uniform_int_distribution<int> dist_short_1_12{1, 12};
  return dist_short_1_12(m_mt);
}

int Random::get0or1() {
  std::uniform_int_distribution<int> dist_short_0_1{0, 1};
  return dist_short_0_1(m_mt);
}

bool Random::getBool() {
  std::uniform_int_distribution<> dist(0, 1);
  return dist(m_mt);
}

std::size_t Random::getInteger0toN(std::size_t n) {
  std::uniform_int_distribution<std::size_t> dist(0, (n - 1));
  return dist(m_mt);
}

std::string Random::getString(std::size_t n) {
  const constexpr std::array<const char, 26> ch_letters_lower{
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  std::string str(n, 0);
  std::uniform_int_distribution<std::size_t> dist(0, ch_letters_lower.size() - 1);
  auto rand_char = [&dist, &rng = m_mt, &ch_letters_lower]() {
    return ch_letters_lower[dist(rng)];
  };
  std::generate_n(str.begin(), n, rand_char);
  return str;
}