#pragma once

#include "Utils.hpp"



namespace iggy{
  class Token{
  public:
    enum Type{
      None,
      IntLit, CharLit, StrLit, FloatLit,
      LBrace, RBrace, LStart, REnd,
      Id, ArithOp, BitOp, CmpOp
    } type;
    std::u8string value;
    uint32_t col, pos;


    std::string to_str() const;
  };
  

  class Lexer{
  public:
    IggyFile file;
    std::string line;
    uint32_t pos, lineno;
    Token token;
    int state;


    explicit Lexer(const std::string&);

    const Token& next();

    const Token& top() const;

    bool ok() const;
  };
}