#pragma once

#include <cstdio>
#include <optional>
#include <string>
#include <unordered_map>

struct BadCharTable {
  std::unordered_map<char, size_t> table;
  size_t patternSize;

  BadCharTable(const std::string &text, const std::string &pattern) {
    for (size_t i = 0; i < pattern.size() - 1; i++) {
      table[pattern[i]] = pattern.size() - 1 - i;
    }
    patternSize = pattern.size();
  }

  size_t shift(char c) {
    auto it = table.find(c);
    if (it != table.end()) {
      return it->second;
    }
    return patternSize;
  }
};

std::optional<size_t> bmhSearch(const std::string &text,
                                const std::string &pattern) {
  if (pattern.size() > text.size() || pattern.empty()) {
    return std::nullopt;
  }

  size_t i = pattern.size() - 1;
  size_t n = i;
  BadCharTable badCharTable(text, pattern);

  while (i < text.size()) {
    if (text.substr(i - pattern.size() + 1, pattern.size()) == pattern) {
      return i - pattern.size() + 1;
    }

    i += badCharTable.shift(text[i]);
  }

  return std::nullopt;
}
