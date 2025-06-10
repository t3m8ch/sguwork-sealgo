#pragma once

#include <string>
#include <vector>

std::vector<int> zFunc(const std::string &s) {
  std::vector<int> zf(s.size(), 0);
  int left = 0;
  int right = 0;

  for (int i = 1; i < s.size(); i++) {
    zf[i] = std::max(0, std::min(right - i, zf[i - left]));

    while (i + zf[i] < s.size() && s[zf[i]] == s[i + zf[i]]) {
      zf[i]++;
    }

    if (i + zf[i] > right) {
      left = i;
      right = i + zf[i];
    }
  }

  return zf;
}
