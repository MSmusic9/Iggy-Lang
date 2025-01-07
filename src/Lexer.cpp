#include "Utils.hpp"
#include "Lexer.hpp"
#include <stdexcept>



namespace iggy{
  std::string Token::to_str() const{
    return std::string((const char*)this->value.c_str());
  }


  Lexer::Lexer(const std::string& file): state(1), file(file, "r"), pos(0), lineno(1){
    if (!this->file.line(this->line)) this->state = 0;
  }


  const Token& Lexer::next(){
    if (this->state){
      uint32_t curr = this->pos, start;
      std::u8string acc;

      // skip whitespaces
      while (this->line[curr] == ' ' || this->line[curr] == '\t' || this->line[curr] == '\r') ++curr;
    
      // skip newline
      if (this->line[curr] == '\n'){
        ++this->lineno;
        this->pos = 0;
        this->state = this->file.line(this->line);
        return this->next();
      }

      // skip one-line comments
      else if (this->line[curr] == '/'){
        ++curr;

        if (this->line[curr] == '/'){
          curr = this->line.size();
        }else{
          this->pos = curr;
          this->token = Token{Token::Type::ArithOp, std::u8string(1, this->line[curr]), this->lineno, curr};
          return this->top();
        }
      }

        // float or integer
        else if (isdigit(this->line[curr])){
          acc = std::u8string();
          bool is_dot = false;

          while (isdigit(this->line[curr]) || this->line[curr] == '.' || this->line[curr] == '_'){
            if (this->line[curr] == '.' && is_dot) this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Double dot in number "s + std::string((const char*)acc.c_str()), this->line);
            else if (this->line[curr] == '.') is_dot = true;

            if (this->line[curr] != '_') acc += this->line[curr];
            ++curr;
          }

          this->pos = curr;
          this->token = Token{(is_dot ? Token::Type::FloatLit : Token::Type::IntLit), acc, this->lineno, start};
          return this->top();
        }

        // Arithmetic operators
        else if (is_in("+-*%", this->line[curr])){
          start = curr;

          if (this->line[curr] == '*' && this->line[curr + 1] == '*'){
            this->pos = ++curr;
            this->token = Token{Token::Type::ArithOp, u8"**", this->lineno, start};
            return this->top();
          }else{
            this->pos = curr;
            this->token = Token{Token::Type::ArithOp, std::u8string(1, this->line[curr]), this->lineno, curr};
            return this->top();
          }
        }

        // Bit operators
        else if (is_in("|&!^~><", this->line[curr])){
          start = curr;

          if (this->line[curr] == '<'){
            if (this->line[curr + 1] == '<'){
              this->pos = ++curr;
              this->token = Token{Token::Type::BitOp, u8"<<", this->lineno, start};
              return this->top();
            }else if (this->line[curr + 1] == '='){
              this->pos = ++curr;
              this->token = Token{Token::Type::CmpOp, u8"<=", this->lineno, start};
              return this->top();
            }else{
              this->pos = curr;
              this->token = Token{Token::Type::CmpOp, u8"<", this->lineno, start};
              return this->top();
            }
          }
          
          else if (this->line[curr] == '>'){
            start = curr;

            if (this->line[curr + 1] == '>'){
              this->pos = ++curr;
              this->token = Token{Token::Type::BitOp, u8">>", this->lineno, start};
              return this->top();
            }else if (this->line[curr + 1] == '=') {
              this->pos = ++curr;
              this->token = Token{Token::Type::CmpOp, u8">=", this->lineno, start};
              return this->top();
            }else{
              this->pos = curr;
              this->token = Token{Token::Type::CmpOp, u8">", this->lineno, start};
              return this->top();
            }
          }
          else{
            this->pos = curr;
            this->token = Token{Token::Type::ArithOp, std::u8string(1, this->line[curr]), this->lineno, start};
            return this->top();
          }
        }

        // Compare operators
        else if (this->line[curr] == '='){
          if (this->line[curr + 1] == '='){
            this->pos = ++curr;
            this->token = Token{Token::Type::CmpOp, u8"==", this->lineno, curr};
            return this->top();
          }

          this->pos = curr;
          this->token = Token{Token::Type::ArithOp, u8"=", this->lineno, curr};
          return this->top();
        }

        // string literal
        else if (this->line[curr] == '"'){
          acc = std::u8string();
          start = curr;
          ++curr;

          while (this->line[curr] != '"'){
            if (this->line[curr] == '\\'){
              ++curr;

              if (this->line[curr] == 'n') acc += u8'\n';
              else if (this->line[curr] == 'r') acc += u8'\r';
              else if (this->line[curr] == 't') acc += u8'\t';
              else if (this->line[curr] == '0') acc += u8'\0';
              else if (this->line[curr] == '"') acc += u8'"';
              else if (this->line[curr] == '\\') acc += u8'\\';
              else if (this->line[curr] == 'u'){
                try{
                  acc += (char8_t)std::stoi(this->line.substr(curr, 3));
                  curr += 3;
                }catch (std::invalid_argument& e){
                  this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Invalid \\u argument "s + std::string(1, this->line[curr]), this->line);
                }
              }
              else if (this->line[curr] == 'x'){
                try{
                  acc += (char8_t)std::stoi(this->line.substr(curr, 2), 0, 16);
                  curr += 2;
                }catch (std::invalid_argument& e){
                  this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Invalid \\x argument "s + std::string(1, this->line[curr]), this->line);
                }
              }
              else this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Invalid escape sequence '"s + std::string(1, this->line[curr]) + '\'', this->line);
            
            }else acc += this->line[curr];

            ++curr;
          }

          this->pos = curr;
          this->token = Token{Token::Type::StrLit, acc, this->lineno, start};
          return this->top();
        }

        // Left brace
        else if (this->line[curr] == '(' || this->line[curr] == '{' || this->line[curr] == '['){
          this->token = Token{Token::Type::LBrace, std::u8string(1, (char8_t)this->line[curr]), this->lineno, curr};
          this->pos = ++curr;
          return this->top();
        }

        // Right brace
        else if (this->line[curr] == ')' || this->line[curr] == '}' || this->line[curr] == ']'){
          this->token = Token{Token::Type::RBrace, std::u8string(1, (char8_t)this->line[curr]), this->lineno, curr};
          this->pos = ++curr;
          return this->top();
        }

        // Char literal
        else if (this->line[curr] == '\''){
          acc = std::u8string();
          start = curr;
          ++curr;

          while (this->line[curr] != '\''){
            if (this->line[curr] == '\\'){
              ++curr;

              if (this->line[curr] == 'n') acc += u8'\n';
              else if (this->line[curr] == 'r') acc += u8'\r';
              else if (this->line[curr] == 't') acc += u8'\t';
              else if (this->line[curr] == '0') acc += u8'\0';
              else if (this->line[curr] == '\'') acc += u8'\'';
              else if (this->line[curr] == '\\') acc += u8'\\';
              else if (this->line[curr] == 'u'){
                try{
                  acc += (char8_t)std::stoi(this->line.substr(curr, 3));
                  curr += 3;
                }catch (std::invalid_argument& e){
                  this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Invalid \\u argument "s + std::string(1, this->line[curr]), this->line);
                }
              }
              else if (this->line[curr] == 'x'){
                try{
                  acc += (char8_t)std::stoi(this->line.substr(curr, 2), 0, 16);
                  curr += 3;
                }catch (std::invalid_argument& e){
                  this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Invalid \\x argument "s + std::string(1, this->line[curr]), this->line);
                }
              }
              else this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Invalid escape sequence "s + std::string(1, this->line[curr]), this->line);

              ++curr;
            }

            else{
              acc += this->line[curr];
              ++curr;
            }
          }

          this->pos = curr;
          this->token = Token{Token::Type::CharLit, acc, this->lineno, start};
          return this->top();
        }

        // Identificator
        else if (isalpha(this->line[curr]) || this->line[curr] == '_'){
          acc = std::u8string(1, this->line[curr]);
          start = curr;
          ++curr;

          while (isalnum(this->line[curr]) || this->line[curr] == '_'){
            acc += this->line[curr];
            ++curr;
          }

          this->pos = curr;
          this->token = Token{Token::Type::Id, acc, this->lineno, start};
          return this->top();
        }

      this->state = raise(this->file, false, this->lineno, curr, "Syntax Error", "Invalid symbol "s + std::string(1, this->line[curr]), this->line);
    }

    return this->token;
  }


  bool Lexer::ok() const{
    return this->state > 0;
  }


  const Token& Lexer::top() const{
    if (this->state == 0) exit(raise(this->file, false, 0, 0, "Syntax Error", "unexpected end of file", this->line));
    return this->token;
  }
}