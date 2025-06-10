#include "kmp.hpp"
#include <iostream>

int main() {
  std::string text;
  std::string pattern;

  std::cout << "Enter text: ";
  std::cin >> text;

  std::cout << "Enter pattern: ";
  std::cin >> pattern;

  auto result = kmpSearch(text, pattern);
  if (!result) {
    std::cout << "Pattern not found." << std::endl;
  } else {
    std::cout << "Pattern found at index " << *result << std::endl;
  }
}
