#ifndef TOKENS_HPP
#define TOKENS_HPP

enum class Token {
  Id,
  Number,
  Sin,
  Cos,
  Tan,
  Asin,
  Acos,
  Atan,
  Log,
  Sqrt,
  Int,
  Equal_to,
  Not_equal_to,
  Less_than,
  Greater_than,
  True,
  False,
  And,
  Or,
  Not,
  Var,
  Print,
  Semicolon = ';',
  Assign = '=',
  Plus = '+',
  Minus = '-',
  Mul = '*',
  Div = '/',
  Mod = '%',
  Pow = '^',
  Lp = '(',
  Rp = ')',
  EOF_sym = -1
};

#endif