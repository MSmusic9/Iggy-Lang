#pragma once

#include <stdio.h>
#include <string>



namespace iggy{
  class IggyFile{
  public:
    FILE* stream;
    char buff[256] = {0};
    std::string name;


    IggyFile(const std::string&, const char*);

    ~IggyFile();

    bool line(std::string&);
  };
}