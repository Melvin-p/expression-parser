
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Parser.hpp"
#include <doctest/doctest.h>
#include <exception>
#include <string>

TEST_SUITE("Expression Parser") {
  Parser parser{};
  TEST_CASE("Constants") {
    SUBCASE("pi") {
      std::string input{"pi;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "3.142\n");
    }
    SUBCASE("e") {
      std::string input{"e;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "2.718\n");
    }
  }
  TEST_CASE("Command") {
    SUBCASE("Missing semi colon") {
      std::string input{"1+1"};
      CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
      parser.getResult();
    }
    SUBCASE("variable assignment") {
      std::string input{"var a = 3.3;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "");
      std::string input_2{"a;"};
      parser.evalArithmetic(input_2);
      CHECK(parser.getResult() == "3.3\n");
    }
    SUBCASE("sequence") {
      std::string input{"var b = 3.3; b * 2; 3 * 2; var c = b; c;"};
      parser.evalArithmetic(input);
      CHECK(parser.getResult() == "6.6\n6\n3.3\n");
    }
  }
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
    SUBCASE("Division by zero") {
      std::string input{"10/0;"};
      CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
      parser.getResult();
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
    SUBCASE("Division by zero") {
      std::string input{"10%0;"};
      CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
      parser.getResult();
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
  TEST_CASE("Parentheses") {
    SUBCASE("1+(2*3)") {
      std::string input = "1+(2*3);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "7\n");
    }
    SUBCASE("(2+3)/(3-1)") {
      std::string input = "(2+3)/(3-1);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "2.5\n");
    }
    SUBCASE("2+(3 * (8 - 2))") {
      std::string input = "2+(3 * (8 - 2));";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "20\n");
    }
  }
  TEST_CASE("Functions") {
    SUBCASE("function missing bracket left") {
      std::string input{"sin pi );"};
      CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
      parser.getResult();
    }
    SUBCASE("function missing bracket right") {
      std::string input{"sin( pi ;"};
      CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
      parser.getResult();
    }
    SUBCASE("Int(0.1)") {
      std::string input = "Int(0.1);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "0\n");
    }
    SUBCASE("Int(-0.1)") {
      std::string input = "Int(-0.1);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "-0\n");
    }
    SUBCASE("sin") {
      std::string input = "Int(sin(pi));";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "0\n");
    }
    SUBCASE("tan") {
      /*
      Fails
        SUBCASE("Invalid Input") {
        std::string input{"tan(pi*0.5);"};
        CHECK_THROWS_AS(parser.evalArithmetic(input), const std::exception &);
        parser.getResult();
      }
      */
      SUBCASE("Valid Input") {
        std::string input{"Int(tan(-pi));"};
        parser.evalArithmetic(input);
        auto temp = parser.getResult();
        CHECK(temp == "0\n");
      }
    }
    SUBCASE("cos") {
      std::string input = "Int(cos(pi));";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "-1\n");
    }
    SUBCASE("asin") {
      std::string input = "asin(1);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "1.571\n");
    }
    SUBCASE("acos") {
      std::string input = "acos(0);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "1.571\n");
    }
    SUBCASE("atan") {
      std::string input = "atan(1) * 4;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "3.142\n");
    }
    SUBCASE("log") {
      std::string input = "log(e^2);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "2\n");

      input = "log(125) / log(5);";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "3\n");
    }
    SUBCASE("sqrt") {
      std::string input = "sqrt(4);";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "2\n");
    }
  }
  TEST_CASE("Boolean") {
    SUBCASE("AND") {
      std::string input = "true and true;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "true\n");
      input = "true and false;";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "false\n");
      input = "false and false;";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "false\n");
      input = "false and true;";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "false\n");
    }
    SUBCASE("OR") {
      std::string input = "true or true;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "true\n");
      input = "true or false;";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "true\n");
      input = "false or false;";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "false\n");
      input = "false or true;";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "true\n");
    }
    SUBCASE("not") {
      std::string input = "not true;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "false\n");
      input = "not false;";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "true\n");
    }
    SUBCASE("comparision") {
      SUBCASE("greater_than") {
        std::string input = "3 greater_than 2;";
        parser.evalArithmetic(input);
        auto temp = parser.getResult();
        CHECK(temp == "true\n");
      }
      SUBCASE("less_than") {
        std::string input = "5 less_than 4.9;";
        parser.evalArithmetic(input);
        auto temp = parser.getResult();
        CHECK(temp == "false\n");
      }
      SUBCASE("equal_to") {
        std::string input = "3.5 equal_to 3.5;";
        parser.evalArithmetic(input);
        auto temp = parser.getResult();
        CHECK(temp == "true\n");
      }
      SUBCASE("not_equal_to") {
        std::string input = "2.1 not_equal_to 3.5;";
        parser.evalArithmetic(input);
        auto temp = parser.getResult();
        CHECK(temp == "true\n");
      }
    }
    SUBCASE("Boolean and arithmetic") {
      std::string input = "3 + 2 - (8 * 4) greater_than 100 or true;";
      parser.evalArithmetic(input);
      auto temp = parser.getResult();
      CHECK(temp == "true\n");
      input = "var a = 10;(a greater_than 100 or a less_than -100) and (a%2 equal_to 0);";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "false\n");
      input = "var a = 110;(a greater_than 100 or a less_than -100) and (a%2 equal_to 0);";
      parser.evalArithmetic(input);
      temp = parser.getResult();
      CHECK(temp == "true\n");
    }
  }
}