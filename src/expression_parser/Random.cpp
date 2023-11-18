#include "Random.hpp"
#include <algorithm>
#include <array>

ActionTokens Random::getArithBinaryOp() {
  const constexpr std::array<const ActionTokens, 6> operators{
      ActionTokens::Addition, ActionTokens::Subtraction, ActionTokens::Multiplication,
      ActionTokens::Modulo,   ActionTokens::Division,    ActionTokens::Power};

  std::uniform_int_distribution<std::size_t> operator_choice{0, operators.size() - 1};
  return operators[operator_choice(m_mt)];
}

ActionTokens Random::getArithUnaryOp() {
  const constexpr std::array<const ActionTokens, 2> operators{ActionTokens::positive,
                                                              ActionTokens::negative};
  std::uniform_int_distribution<std::size_t> operator_choice{0, operators.size() - 1};
  return operators[operator_choice(m_mt)];
}

ActionTokens Random::getFunction() {
  const constexpr std::array<const ActionTokens, 9> functions{
      ActionTokens::sin,  ActionTokens::cos,  ActionTokens::tan,
      ActionTokens::Asin, ActionTokens::Atan, ActionTokens::Acos,
      ActionTokens::Log,  ActionTokens::Sqrt, ActionTokens::Int};

  std::uniform_int_distribution<std::size_t> operator_choice{0, functions.size() - 1};
  return functions[operator_choice(m_mt)];
}

ActionTokens Random::getBinaryOp() {
  const constexpr std::array<const ActionTokens, 2> operators{ActionTokens::And, ActionTokens::Or};
  std::uniform_int_distribution<std::size_t> operator_choice{0, operators.size() - 1};
  return operators[operator_choice(m_mt)];
}

ActionTokens Random::getCompOp() {
  const constexpr std::array<const ActionTokens, 4> operators{
      ActionTokens::Greater_than, ActionTokens::Less_than, ActionTokens::Equal_to,
      ActionTokens::Not_equal_to};
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