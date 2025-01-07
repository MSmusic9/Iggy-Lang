#include "Utils.hpp"



namespace iggy{
  bool is_in(std::string str, char value){
    for (char& i: str){
      if (i == value) return true;
    }

    return false;
  }


  int raise(const IggyFile& file, bool type, uint32_t line, uint32_t symbol, const std::string& name, const std::string& message, const std::string& code){
    // type: 0(error) or 1(warning)
    std::cerr << "At " << file.name << ": line " << line << ", symbol " << symbol << '\n';

    std::cerr << (type ? "\033[30m(Warning) " : "\033[31m(Error) ") << name << ": " << message << "\033[0m\n";

    if (code[0] != '\0'){
      std::cerr << line << " |" << "\t" << code << '\n';
      std::cerr << "    \t" << std::string(symbol, ' ') << "\033[30m^\033[0m\n";
    }

    std::cerr << std::endl;
    return -1;
  }


  std::string mangle(const std::string& name, const MangleInfo& info){
    std::string res = "_Z";

    if (info.mod.size() > 0){
      res += 'N';

      std::string nsp;
      int pos = 0, iter = 0;

      for (; iter < info.mod.size(); ++iter){
        if (info.mod[iter] == '.'){
          nsp = info.mod.substr(pos, iter - pos);

          res += std::to_string(nsp.size()) + nsp;

          pos = iter + 1;
        }
      }

      nsp = info.mod.substr(pos, iter - pos);
      res += std::to_string(nsp.size()) + nsp;

      res += std::to_string(name.size()) + name;
      res += 'E';
    }
    
    
    if (info.args.size() > 0){
      
    }
    else res += 'v';

    return res;
  }
}