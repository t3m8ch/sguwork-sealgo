#include "prefix-func.hpp"
#include "print-vec.hpp"
#include "z-func.hpp"
#include <iostream>
#include <string>

int main() {
  std::string s;
  getline(std::cin, s);

  std::cout << "Строка:\t\t\t" << s << std::endl;
  std::cout << "Префикс-функция:\t" << printVec(prefixFunc(s)) << std::endl;
  std::cout << "Z-функция:\t\t" << printVec(zFunc(s)) << std::endl;

  return 0;
}
