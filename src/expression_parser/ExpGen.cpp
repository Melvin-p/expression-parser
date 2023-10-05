#include "ExpGen.hpp"
#include "AST.hpp"
#include "Node.hpp"
#include "Random.hpp"
#include "tokens.hpp"
#include <cassert>
#include <cstddef>
#include <memory>

namespace {
const constexpr std::size_t max_iterations{10};

} // namespace

std::unique_ptr<Arithmetic> ExpGen::genArithmetic(const double prob, bool unary) {
  // random increasing probability with each recursion a root node is choosen
  double p{m_random.getReal0to1()};
  if (p > prob) {
    bool var{m_random.getBool()};
    if (var && !m_doubles.empty()) {
      auto it{std::next(m_doubles.begin(), m_random.getInteger0toN(m_doubles.size()))};
      return std::make_unique<Variable>(*it);
    } else {
      return std::make_unique<AtomicArithmetic>(m_random.getReal0to9());
    }
  } else {
    auto arith_prob{m_random.getInteger1to12()};
    /*
    50% chance of first two conditions if unary is false
    33.3% of the first three conditions if unary if true
    */
    if ((arith_prob <= 6 && !unary) || (arith_prob <= 4 && unary)) {

      auto left{genArithmetic(prob / 1.1, unary)};
      Token t{m_random.getArithBinaryOp()};
      auto right{genArithmetic(prob / 1.1, unary)};
      auto out = std::make_unique<BinaryArithmeticOperation>(std::move(left), t, std::move(right));

      if (m_random.getInteger1to12() >= 10) {
        return std::make_unique<ParenthesesArithmetic>(std::move(out));
      } else {
        return out;
      }
    } else if ((arith_prob >= 7 && arith_prob <= 12 && !unary) ||
               (arith_prob >= 5 && arith_prob <= 8 && unary)) {

      auto input{genArithmetic(prob / 1.1, unary)};
      Token t{m_random.getFunction()};
      auto out = std::make_unique<FunctionArithmetic>(std::move(input), t);

      if (m_random.getInteger1to12() >= 10) {
        return std::make_unique<ParenthesesArithmetic>(std::move(out));
      } else {
        return out;
      }
    } else if (arith_prob >= 9 && arith_prob <= 12 && unary) {

      // do not generate any more unary operators
      auto left{genArithmetic(prob / 1.1, false)};
      Token t{m_random.getArithUnaryOp()};
      auto out = std::make_unique<UnaryArithmeticOperation>(std::move(left), t);

      if (m_random.getInteger1to12() >= 10) {
        return std::make_unique<ParenthesesArithmetic>(std::move(out));
      } else {
        return out;
      }
    } else {
      // this code should be unreachable
      assert(false);
      return std::make_unique<AtomicArithmetic>(m_random.getReal0to9());
    }
  }
}

std::unique_ptr<Boolean> ExpGen::genBoolean(const double prob, bool unary) {
  double p{m_random.getReal0to1()};
  if (p > prob) {
    // choose variable or atomic
    bool var{m_random.getBool()};
    if (var && !m_bools.empty()) {
      auto it{std::next(m_bools.begin(), m_random.getInteger0toN(m_bools.size()))};
      return std::make_unique<Variable>(*it);
    } else {
      return std::make_unique<AtomicBoolean>(m_random.get0or1());
    }
  } else {
    auto arith_prob{m_random.getInteger1to12()};
    if ((arith_prob <= 6 && !unary) || (arith_prob <= 4 && unary)) {

      auto left{genBoolean(prob / 1.1, unary)};
      Token t{m_random.getBinaryOp()};
      auto right{genBoolean(prob / 1.1, unary)};

      auto out = std::make_unique<BinaryBooleanOperation>(std::move(left), t, std::move(right));

      if (m_random.getInteger1to12() >= 10) {
        return std::make_unique<ParenthesesBoolean>(std::move(out));
      } else {
        return out;
      }
    } else if ((arith_prob >= 7 && arith_prob <= 12 && !unary) ||
               (arith_prob >= 5 && arith_prob <= 8 && unary)) {

      auto left{genArithmetic(prob / 1.1, unary)};
      Token t{m_random.getCompOp()};
      auto right{genArithmetic(prob / 1.1, unary)};
      auto out = std::make_unique<Comparision>(std::move(left), t, std::move(right));
      return std::make_unique<ParenthesesBoolean>(std::move(out));

    } else if (arith_prob >= 9 && arith_prob <= 12 && unary) {

      // do not generate any more unary operators
      auto left{genBoolean(prob / 1.1, false)};
      Token t{Token::Not}; // currently the only unary boolean token
      auto out = std::make_unique<UnaryBooleanOperation>(std::move(left), t);

      if (m_random.getInteger1to12() >= 10) {
        return std::make_unique<ParenthesesBoolean>(std::move(out));
      } else {
        return out;
      }
    } else {
      // this code should be unreachable
      assert(false);
      return std::make_unique<AtomicBoolean>(m_random.getReal0to9());
    }
  }
}

std::unique_ptr<Program> ExpGen::getStatements(std::size_t count) {
  auto out{std::make_unique<Program>()};
  // produce count number of statements
  for (std::size_t i = 0; i < count; ++i) {
    bool assignment{m_random.getBool()};
    bool new_var{m_random.getBool()};

    // assignment to new variable
    if ((assignment && new_var) || (assignment && !new_var && m_symbol_table.empty())) {
      auto var{getNewVariable()};

      bool type{m_random.getBool()};
      // double
      if (type) {
        auto exp_temp = genArithmetic();
        auto val{std::make_unique<Assignment>(std::move(exp_temp), var)};
        out->append(std::move(val));
        m_doubles.push_back(var);
        m_symbol_table[var] = true;
        // bool
      } else {
        auto exp_temp = genBoolean();
        auto val{std::make_unique<Assignment>(std::move(exp_temp), var)};
        out->append(std::move(val));
        m_bools.push_back(var);
        m_symbol_table[var] = false;
      }
    } else if (assignment && !new_var && !m_symbol_table.empty()) {
      auto it{std::next(m_symbol_table.begin(), m_random.getInteger0toN(m_symbol_table.size()))};
      auto var{it->first};
      auto type{it->second};

      // double
      if (type) {
        auto exp_temp = genArithmetic();
        auto val{std::make_unique<Assignment>(std::move(exp_temp), var)};
        out->append(std::move(val));
        m_symbol_table[var] = true;
        // bool
      } else {
        auto exp_temp = genBoolean();
        auto val{std::make_unique<Assignment>(std::move(exp_temp), var)};
        out->append(std::move(val));
        m_symbol_table[var] = false;
      }
    } else if (!assignment) {
      bool type{m_random.getBool()};
      // double
      if (type) {
        auto exp_temp = genArithmetic();
        auto val{std::make_unique<Print>(std::move(exp_temp))};
        out->append(std::move(val));

        // bool
      } else {
        auto exp_temp = genBoolean();
        auto val{std::make_unique<Print>(std::move(exp_temp))};
        out->append(std::move(val));
      }
    }
  }
  return out;
}

std::string ExpGen::getNewVariable() {
  auto var{m_random.getString()};

  // ensure variable does not already exist
  std::size_t iterations{};
  while (m_symbol_table.find(var) != m_symbol_table.end()) {
    var = m_random.getString();
    ++iterations;
    if (iterations > max_iterations) {
      break;
    }
  }
  return var;
}
