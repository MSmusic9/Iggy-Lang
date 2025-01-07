#pragma once

#include "Lexer.hpp"



namespace iggy{
  const std::unordered_map<std::string, int> OpPrec{
    {"=", 1},
    {">", 11}, {"<", 11}, {">=", 11}, {"<=", 11}, {"==", 11},
    {"+", 21}, {"-", 21}, {"|", 21}, {"^", 21},
    {"*", 31}, {"/", 31}, {"&", 31}, {"%", 31},
  };


  namespace nodes{
    class ParseNode;
    class FuncDefNode;
  }


  class Parser{
  public:
    std::vector<nodes::ParseNode*> ast;
    std::unordered_map<std::string, std::string> defines;
    std::unordered_map<std::string, nodes::FuncDefNode> funcs;
    std::vector<std::string> imports;
    Lexer* lexer;
    std::string space;
    int nested_classes;
    bool state = 1;


    explicit Parser(Lexer*);

    nodes::ParseNode* parsePrimary();
    
    nodes::ParseNode* parseBrace();

    nodes::ParseNode* parseExpr();

    nodes::ParseNode* parseBinary(int, nodes::ParseNode*);

    int getOpPrec();
  };
}