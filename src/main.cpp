#include "Lexer.hpp"
#include "Parser.hpp"
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

int main() {

  int return_value{0};
  std::string buffer{};
  buffer.reserve(1024);

  while (true) {
    std::cout << "something> ";
    std::getline(std::cin, buffer);
    if (buffer == "q") {
      break;
    } else {
      try {
        Parser parser{};
        std::cout << parser(buffer) << "\n";
      } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
      }
    }
  }
  return return_value;
}