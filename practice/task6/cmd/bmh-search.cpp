#include "bmh.hpp"
#include <iostream>

int main() {
  std::string text;
  std::string pattern;

  std::cout << "Enter text: ";
  std::getline(std::cin, text);

  std::cout << "Enter pattern: ";
  std::getline(std::cin, pattern);

  auto result = bmhSearch(text, pattern);
  if (!result) {
    std::cout << "Pattern not found." << std::endl;
  } else {
    std::cout << "Pattern found at index " << *result << std::endl;
  }
}
