#pragma once

#include <string>
#include <vector>

std::vector<int> prefixFunc(const std::string &s) {
  std::vector<int> p(s.size(), 0);

  for (size_t i = 1; i < s.size(); i++) {
    size_t cur = p[i - 1];

    while (s[i] != s[cur] && cur > 0) {
      cur = p[cur - 1];
    }

    if (s[i] == s[cur]) {
      p[i] = cur + 1;
    }
  }

  return p;
}
