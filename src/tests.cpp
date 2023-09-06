#include <exception>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Parser.hpp"
#include <doctest/doctest.h>
#include <string>

TEST_SUITE("Expression Parser") {
  Parser parser{};
  TEST_CASE("Unary") {
    SUBCASE("-1") {
      std::string input{"-1;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "-1\n");
    }
    SUBCASE("+1") {
      std::string input{"+1;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "1\n");
    }
    SUBCASE("--1") {
      std::string input{"--1;"};
      CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
      parser.getResult();
    }
    SUBCASE("+-1") {
      std::string input{"--1;"};
      CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
      parser.getResult();
    }
  }
  TEST_CASE("Addition") {
    SUBCASE("1+1") {
      std::string input{"1+1;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "2\n");
    }
    SUBCASE("-11+34") {
      std::string input{"-11+34;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "23\n");
    }
    SUBCASE("-11+-34") {
      std::string input{"-11+-34;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "-45\n");
    }
  }
  TEST_CASE("Multiplication") {
    SUBCASE("5*2") {
      std::string input{"5*2;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "10\n");
    }
  }
  TEST_CASE("Division") {
    SUBCASE("10/2") {
      std::string input{"10/2;"};
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "5\n");
    }
  }
  TEST_CASE("Subtraction") {
    SUBCASE("3-10") {
      std::string input = "3-10;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "-7\n");
    }

    SUBCASE("-3-10") {
      std::string input = "-3-10;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "-13\n");
    }

    SUBCASE("10-2") {
      std::string input{"10-2;"};
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "8\n");
    }
  }
  TEST_CASE("Modulo") {
    SUBCASE("10%2") {
      std::string input{"10%2;"};
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "0\n");
    }

    SUBCASE("9%2") {
      std::string input = "9%2;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "1\n");
    }
  }
  TEST_CASE("Power") {
    SUBCASE("2^2") {
      std::string input = "2^2;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "4\n");
    }
    SUBCASE("-2^2") {
      std::string input = "-2^2;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "4\n");
    }
    SUBCASE("4^0.5") {
      std::string input = "4^0.5;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "2\n");
    }
  }
}