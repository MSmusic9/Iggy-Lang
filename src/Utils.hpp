#pragma once

#include "IggyFile.hpp"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <string>



namespace iggy{
  using namespace std::string_literals;


  bool is_in(std::string, char);

  int raise(const IggyFile&, bool, uint32_t, uint32_t, const std::string&, const std::string&, const std::string&);


  struct MangleInfo{
    std::string args, mod;
    bool is_method;
  };


  std::string mangle(const std::string&, const MangleInfo&);
}