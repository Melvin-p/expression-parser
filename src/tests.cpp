
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Interpreter.hpp"
#include <doctest/doctest.h>
#include <exception>
#include <string>

TEST_SUITE("Expression Parser") {
  Interpreter interpreter{};
  TEST_CASE("Constants") {
    SUBCASE("pi") {
      std::string input{"pi;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "3.142\n");
    }
    SUBCASE("e") {
      std::string input{"e;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "2.718\n");
    }
  }
  TEST_CASE("Command") {
    SUBCASE("Missing semi colon") {
      std::string input{"1+1"};
      CHECK_THROWS_AS(interpreter.evaluate(input), const std::exception &);
    }
    SUBCASE("variable assignment") {
      std::string input{"var a = 3.3;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "");
    }
    SUBCASE("Print variable") {
      std::string input_2{"a;"};
      auto output = interpreter.evaluate(input_2);
      CHECK(output == "3.3\n");
    }
    SUBCASE("sequence") {
      std::string input{"var b = 3.3; b * 2; 3 * 2; var c = b; c;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "6.6\n6\n3.3\n");
    }
  }
  TEST_CASE("Unary") {
    SUBCASE("-1") {
      std::string input{"-1;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "-1\n");
    }
    SUBCASE("+1") {
      std::string input{"+1;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "1\n");
    }
    SUBCASE("--1") {
      std::string input{"--1;"};
      CHECK_THROWS_AS(interpreter.evaluate(input), const std::exception &);
    }
    SUBCASE("+-1") {
      std::string input{"--1;"};
      CHECK_THROWS_AS(interpreter.evaluate(input), const std::exception &);
    }
  }
  TEST_CASE("Addition") {
    SUBCASE("1+1") {
      std::string input{"1+1;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "2\n");
    }
    SUBCASE("-11+34") {
      std::string input{"-11+34;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "23\n");
    }
    SUBCASE("-11+-34") {
      std::string input{"-11+-34;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "-45\n");
    }
  }
  TEST_CASE("Multiplication") {
    SUBCASE("5*2") {
      std::string input{"5*2;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "10\n");
    }
  }
  TEST_CASE("Division") {
    SUBCASE("10/2") {
      std::string input{"10/2;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "5\n");
    }
    SUBCASE("Division by zero") {
      std::string input{"10/0;"};
      CHECK_THROWS_AS(interpreter.evaluate(input), const std::exception &);
    }
  }
  TEST_CASE("Subtraction") {
    SUBCASE("3-10") {
      std::string input = "3-10;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "-7\n");
    }

    SUBCASE("-3-10") {
      std::string input = "-3-10;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "-13\n");
    }

    SUBCASE("10-2") {
      std::string input{"10-2;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "8\n");
    }
  }
  TEST_CASE("Modulo") {
    SUBCASE("10%2") {
      std::string input{"10%2;"};
      auto output = interpreter.evaluate(input);
      CHECK(output == "0\n");
    }

    SUBCASE("9%2") {
      std::string input = "9%2;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "1\n");
    }
    SUBCASE("Division by zero") {
      std::string input{"10%0;"};
      CHECK_THROWS_AS(interpreter.evaluate(input), const std::exception &);
    }
  }
  TEST_CASE("Power") {
    SUBCASE("2^2") {
      std::string input = "2^2;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "4\n");
    }
    SUBCASE("-2^2") {
      std::string input = "-2^2;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "4\n");
    }
    SUBCASE("4^0.5") {
      std::string input = "4^0.5;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "2\n");
    }
  }
  TEST_CASE("Parentheses") {
    SUBCASE("1+(2*3)") {
      std::string input = "1+(2*3);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "7\n");
    }
    SUBCASE("(2+3)/(3-1)") {
      std::string input = "(2+3)/(3-1);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "2.5\n");
    }
    SUBCASE("2+(3 * (8 - 2))") {
      std::string input = "2+(3 * (8 - 2));";
      auto output = interpreter.evaluate(input);
      CHECK(output == "20\n");
    }
  }
  TEST_CASE("Functions") {
    SUBCASE("function missing bracket left") {
      std::string input{"sin pi );"};
      CHECK_THROWS_AS(interpreter.evaluate(input), const std::exception &);
    }
    SUBCASE("function missing bracket right") {
      std::string input{"sin( pi ;"};
      CHECK_THROWS_AS(interpreter.evaluate(input), const std::exception &);
    }
    SUBCASE("Int(0.1)") {
      std::string input = "Int(0.1);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "0\n");
    }
    SUBCASE("Int(-0.1)") {
      std::string input = "Int(-0.1);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "-0\n");
    }
    SUBCASE("sin") {
      std::string input = "Int(sin(pi));";
      auto output = interpreter.evaluate(input);
      CHECK(output == "0\n");
    }
    SUBCASE("tan") {

      // invalid input to tan fails for some reason

      SUBCASE("Valid Input") {
        std::string input{"Int(tan(-pi));"};
        auto output = interpreter.evaluate(input);
        CHECK(output == "0\n");
      }
    }
    SUBCASE("cos") {
      std::string input = "Int(cos(pi));";
      auto output = interpreter.evaluate(input);
      CHECK(output == "-1\n");
    }
    SUBCASE("asin") {
      std::string input = "asin(1);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "1.571\n");
    }
    SUBCASE("acos") {
      std::string input = "acos(0);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "1.571\n");
    }
    SUBCASE("atan") {
      std::string input = "atan(1) * 4;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "3.142\n");
    }
    SUBCASE("log") {
      std::string input = "log(e^2);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "2\n");

      input = "log(125) / log(5);";
      output = interpreter.evaluate(input);
      CHECK(output == "3\n");
    }
    SUBCASE("sqrt") {
      std::string input = "sqrt(4);";
      auto output = interpreter.evaluate(input);
      CHECK(output == "2\n");
    }
  }
  TEST_CASE("Boolean") {
    SUBCASE("AND") {

      std::string input = "true and true;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "true\n");

      input = "true and false;";
      output = interpreter.evaluate(input);
      CHECK(output == "false\n");

      input = "false and false;";
      output = interpreter.evaluate(input);
      CHECK(output == "false\n");

      input = "false and true;";
      output = interpreter.evaluate(input);
      CHECK(output == "false\n");
    }
    SUBCASE("OR") {
      std::string input = "true or true;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "true\n");

      input = "true or false;";
      output = interpreter.evaluate(input);
      CHECK(output == "true\n");

      input = "false or false;";
      output = interpreter.evaluate(input);
      CHECK(output == "false\n");

      input = "false or true;";
      output = interpreter.evaluate(input);
      CHECK(output == "true\n");
    }
    SUBCASE("not") {
      std::string input = "not true;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "false\n");

      input = "not false;";
      output = interpreter.evaluate(input);
      CHECK(output == "true\n");
    }
    SUBCASE("comparision") {
      SUBCASE("greater_than") {
        std::string input = "3 greater_than 2;";
        auto output = interpreter.evaluate(input);
        CHECK(output == "true\n");
      }
      SUBCASE("less_than") {
        std::string input = "5 less_than 4.9;";
        auto output = interpreter.evaluate(input);
        CHECK(output == "false\n");
      }
      SUBCASE("equal_to") {
        std::string input = "3.5 equal_to 3.5;";
        auto output = interpreter.evaluate(input);
        CHECK(output == "true\n");
      }
      SUBCASE("not_equal_to") {
        std::string input = "2.1 not_equal_to 3.5;";
        auto output = interpreter.evaluate(input);
        CHECK(output == "true\n");
      }
    }
    SUBCASE("Boolean and arithmetic") {
      std::string input = "3 + 2 - (8 * 4) greater_than 100 or true;";
      auto output = interpreter.evaluate(input);
      CHECK(output == "true\n");

      input = "var a = 10;(a greater_than 100 or a less_than -100) and (a%2 equal_to 0);";
      output = interpreter.evaluate(input);
      CHECK(output == "false\n");

      input = "var a = 110;(a greater_than 100 or a less_than -100) and (a%2 equal_to 0);";
      output = interpreter.evaluate(input);
      CHECK(output == "true\n");
    }
  }
}