#include "prefix-func.hpp"
#include "z-func.hpp"
#include <iostream>
#include <string>

std::string printVec(const std::vector<int> &vec) {
  std::string result = "";
  for (size_t i = 0; i < vec.size(); ++i) {
    result += std::to_string(vec[i]);
  }
  return result;
}

int main() {
  std::string s;
  getline(std::cin, s);

  std::cout << "Строка:\t\t\t" << s << std::endl;
  std::cout << "Префикс-функция:\t" << printVec(prefixFunc(s)) << std::endl;
  std::cout << "Z-функция:\t\t" << printVec(zFunc(s)) << std::endl;

  return 0;
}
