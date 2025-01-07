#include "Nodes.hpp"



namespace iggy{
  namespace nodes{
    ParseNode* IntegerNode::parse(Parser& ctx){
      std::string i = ctx.lexer->next().to_str();

      return new IntegerNode(std::stoi(i));
    }


    ParseNode* FloatNode::parse(Parser& ctx){
      std::string i = ctx.lexer->next().to_str();

      return new FloatNode(std::stod(i));
    }


    ParseNode* StringNode::parse(Parser& ctx){
      return new StringNode(ctx.lexer->next().value);
    }


    ParseNode* VariableNode::parse(Parser& ctx){
      std::string i = ctx.lexer->next().to_str();

      return new VariableNode(ctx.space, i);
    }


    ParseNode* DefineNode::parse(Parser& ctx){
      ctx.lexer->next();
      std::string name = ctx.lexer->next().to_str();

      ctx.lexer->next();

      ParseNode* val = ctx.parseExpr();
      ctx.lexer->next();

      return new DefineNode(ctx.space, name, val);
    }
  }
}