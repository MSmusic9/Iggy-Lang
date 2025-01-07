#include "src/Utils.cpp"
#include "src/Lexer.cpp"
#include "src/IggyFile.cpp"
/*#include "src/Nodes.cpp"
#include "src/Parser.cpp"*/
#include <chrono>



int main(){
  iggy::Lexer lexer = iggy::Lexer("test.iggy");
  while (lexer.ok()) std::cout << lexer.next().to_str() << std::endl;
  //*((int*)0xff) = 8;
}