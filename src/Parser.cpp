#include "Parser.hpp"
#include "Nodes.hpp"



namespace iggy{
  nodes::ParseNode* Parser::parseBrace(){
    this->lexer->next();
    nodes::ParseNode* node = parseExpr();

    Token rbrace = this->lexer->next();

    if (rbrace.type != Token::Type::RBrace) this->state = raise(this->lexer->file, false, rbrace.col, rbrace.pos, "Syntax Error", "expected ')' at the end.", this->lexer->line);

    return nullptr;
  }


  nodes::ParseNode* Parser::parsePrimary(){
    const Token& curr = this->lexer->top();

    switch (curr.type){
      case Token::Type::IntLit: return nodes::IntegerNode::parse(*this);

      case Token::Type::FloatLit: return nodes::FloatNode::parse(*this);

      case Token::Type::StrLit: return nodes::StringNode::parse(*this);

      case Token::Type::Id: return nodes::VariableNode::parse(*this);

      case Token::Type::LBrace: return this->parseBrace();

      default:
        this->state = raise(this->lexer->file, false, curr.col, curr.pos, "Syntax Error", "unexpected token "s + curr.to_str(), this->lexer->line);
    }

    return nullptr;
  }


  nodes::ParseNode* Parser::parseExpr(){
    if (this->lexer->state < 0) exit(-1);
    nodes::ParseNode* arg = this->parsePrimary();
    return this->parseBinary(0, arg);
  }


  nodes::ParseNode* Parser::parseBinary(int prec, nodes::ParseNode* lhs){
    int curr_prec, next_prec;
    Token op;
    nodes::ParseNode* rhs;

    while (true){
      curr_prec = this->getOpPrec();
      if (curr_prec < prec) return lhs;

      op = this->lexer->next();

      rhs = this->parsePrimary();
      if (!rhs) return nullptr;

      next_prec = this->getOpPrec();

      if (curr_prec < next_prec){
        rhs = this->parseBinary(curr_prec + 10, rhs);
        if (!rhs) return nullptr;
      }

      lhs = new nodes::BinaryOpNode(op.to_str(), rhs, lhs);
    }
  }


  int Parser::getOpPrec(){
    const Token& curr = this->lexer->top();

    try{
      return OpPrec.at(curr.to_str());
    }catch (const std::out_of_range& e){
      this->state = raise(this->lexer->file, false, curr.col, curr.pos, "Syntax Error", "undefined binary operator "s + curr.to_str(), this->lexer->line);
    }

    return 0;
  }


  Parser::Parser(Lexer* lexer){
    this->lexer = lexer;
    
    while (this->lexer->ok()) this->ast.push_back(parseExpr());
  }
}