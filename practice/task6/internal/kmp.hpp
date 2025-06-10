#pragma once

#include "prefix-func.hpp"
#include <optional>

std::optional<size_t> kmpSearch(const std::string &text,
                                const std::string &pattern) {
  size_t j = 0;
  auto pi = prefixFunc(pattern);

  for (size_t i = 0; i < text.size(); i++) {
    while (j > 0 && text[i] != pattern[j]) {
      j = pi[j - 1];
    }

    if (text[i] == pattern[j]) {
      j++;
    }

    if (j >= pattern.size()) {
      return std::optional<size_t>(i - j + 1);
    }
  }

  return std::nullopt;
}
