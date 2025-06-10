#pragma once

#include <string>
#include <vector>

std::string printVec(const std::vector<int> &vec) {
  std::string result = "";
  for (size_t i = 0; i < vec.size(); ++i) {
    result += std::to_string(vec[i]);
  }
  return result;
}
