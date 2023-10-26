#include "ExpGen.hpp"
#include "Node.hpp"
#include <cstddef>
#include <exception>
#include <iostream>

// #define NULL_STREAM

#ifndef NULL_STREAM
#define OUT std::cout
#else
#define OUT null_stream
#endif

namespace {
class NullBuffer : public std::streambuf {
public:
  int overflow(int c) { return c; }
};
NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);
} // namespace

void gen_and_parse() {
  for (std::size_t i = 0; i < 100000; ++i) {
    ExpGen exp_gen{};
    auto val{exp_gen.getStatements()};
    std::string input{val->toString(false)};
    Node::SymbolTable s{};
    try {
      auto output = val->eval(s);
      OUT << "INPUT\n";
      OUT << input << "\n";
      OUT << "OUTPUT\n";
      OUT << output << "\n";
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
      continue;
    }
  }
}

void gen() {
  for (std::size_t i = 0; i < 100; ++i) {
    ExpGen exp_gen{};
    auto val{exp_gen.getStatements()};
    std::string input{val->toString(false)};
    OUT << input << "\n";
  }
}

int main() {
  gen_and_parse();
  return 0;
}
