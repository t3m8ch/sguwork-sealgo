#import "../conf.typ": conf

#show: conf.with(
  meta: (
    title: "Анализ алгоритма Бойера—Мура",
    author: "Кудяков Артём Александрович",
    group: "251",
    city: "Саратов",
    year: 2025,
  )
)

= Код программы

```cpp
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
```

#pagebreak()

== Анализ сложности

+ Пусть $n$ --- длина текста, $k$ --- длина паттерна.
+ *Преобработка* \
  Строим таблицу сдвигов, пробегаясь по всем символам из паттерна за $O(k)$.
+ *Лучший случай* \
  + Происходит, когда паттерн не встречается в тексте и мы делаем сдвиг = $k$.
  + В таком случае *сложность* $O(n / k)$
+ *Худший случай* \
  + Происходят при паттернах типа "aaa...ab" в тексте "aaa...aaa"
  + В таком случае несовпадение происходит на последнем символе паттерна, поэтому сдвиг = $1$.
  + Кроме того, операция сравнения строк имеет сложность $O(k)$ (а строки мы сравниваем после каждого сдвига).
  + В таком случае *сложность* $O(n k)$.
+ *Средний случай* \
  + Происходит при случайном тексте и паттерне.
  + В таком случае *сложность* $O(n)$.
