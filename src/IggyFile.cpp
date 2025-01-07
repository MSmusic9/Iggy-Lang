#include "IggyFile.hpp"
#include "Utils.hpp"



namespace iggy{
  IggyFile::IggyFile(const std::string& path, const char* mode){
    this->name = path;

    this->stream = fopen(path.c_str(), mode);
    if (!this->stream) raise(*this, false, 0, 0, "Cli error", "No such file "s + path, "\0");
  }


  IggyFile::~IggyFile(){
    fclose(this->stream);
  }


  bool IggyFile::line(std::string& ln){
    if (fgets(this->buff, 255, this->stream)){
      ln = this->buff;
      return true;
    }
    
    return false;
  }
}