#include "Lexer.hpp"
#include "Parser.hpp"
#include <exception>
#include <iostream>
#include <string>

int main() {

  int return_value{0};
  std::string buffer{};
  buffer.reserve(1024);
  Parser parser{};

  while (true) {
    std::cout << "input> ";
    std::getline(std::cin, buffer);
    if (buffer == "q") {
      break;
    } else {
      try {
        std::cout << parser(buffer) << "\n";
      } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
      }
    }
  }
  return return_value;
}