#pragma once

#include "Parser.hpp"



namespace iggy{
  namespace nodes{
    class ParseNode{
    public:
      ParseNode() = default;

      virtual ~ParseNode(){};

      //virtual //void compile(const Parser&) = 0;

      static ParseNode* parse(Parser&);
    };


    class IntegerNode: public ParseNode{
    public:
      int32_t value;


      IntegerNode(int32_t val): value(val){}

      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class FloatNode: public ParseNode{
    public:
      double value;


      FloatNode(double val): value(val){}

      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class StringNode: public ParseNode{
    public:
      std::u8string value;


      StringNode(const std::u8string& val): value(val){}

      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class VariableNode: public ParseNode{
    public:
      std::string space, name;


      VariableNode(std::string space, std::string name): space(space), name(name){}

      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class DefineNode: public ParseNode{
    public:
      std::string space, name;
      ParseNode* value;


      DefineNode(std::string space, std::string name, ParseNode* val): space(space), name(name), value(val){}

      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class BinaryOpNode: public ParseNode{
    public:
      std::string op;
      ParseNode* arg1, *arg2;


      BinaryOpNode(const std::string& op, ParseNode* arg1, ParseNode* arg2): op(op), arg1(arg1), arg2(arg2){}

      //void compile(const Parser&);
    };


    class UnaryOpNode: public ParseNode{
    public:
      std::string op;
      ParseNode* arg;


      UnaryOpNode(const std::string& op, ParseNode* arg): op(op), arg(arg){}

      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class ProtoNode: public ParseNode{
    public:
      std::string space, name;
      std::vector<VariableNode*> args;
      std::vector<std::string> decorators;
      std::vector<ParseNode*> templates;


      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class FuncDefNode: public ParseNode{
    public:
      ProtoNode* proto;
      std::vector<ParseNode*> body;


      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class ClassNode: public ParseNode{
    public:
      std::string space, name;
      std::vector<VariableNode*> members;
      std::vector<std::string> decorators;
      std::vector<ParseNode*> templates;


      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };


    class ComplexOpNode: public ParseNode{
    public:
      ParseNode* object;
      char start_op, end_op;
      std::vector<ParseNode*> args;
      std::vector<ParseNode*> templates;


      //void compile(const Parser&);

      static ParseNode* parse(Parser&);
    };
  }
}