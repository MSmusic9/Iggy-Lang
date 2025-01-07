#include "src/Utils.cpp"



int main(){
  auto mangle = iggy::MangleInfo{"", "libiggy.abi.private", true};

  std::cout << iggy::mangle("hello", mangle) << std::endl;
}