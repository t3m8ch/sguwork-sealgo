#import "../conf.typ": conf

#show: conf.with(
  meta: (
    title: "Анализ сложности быстрой и пирамидальной сортировок",
    author: "Кудяков Артём Александрович",
    group: "251",
    city: "Саратов",
    year: 2025,
  )
)

= Код программы

```cpp
#include <concepts>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>
#include <map>

namespace BinTree {

template <std::totally_ordered T>
struct Node {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    T value;

    explicit Node(T value) : value(value), left(nullptr), right(nullptr) {}
};

template <std::totally_ordered T>
static void addNode(std::unique_ptr<Node<T>> &node, const T& value) {
    if (node == nullptr) {
        node = std::make_unique<Node<T>>(value);
    } else if (node->value > value) {
        addNode(node->left, value);
    } else if (node->value == value) {
        return;
    } else {
        addNode(node->right, value);
    }
}

template <std::totally_ordered T>
static void _removeRecursive(std::unique_ptr<Node<T>> &a, std::unique_ptr<Node<T>> &b) {
    if (b->right != nullptr) {
        _removeRecursive(a, b->right);
    } else {
        a->value = b->value;
        b = std::move(b->left);
    }
}

template <std::totally_ordered T>
static void removeNode(std::unique_ptr<Node<T>> &node, const T& value) {
    if (node == nullptr) {
        return;
    }

    if (node->value > value) {
        removeNode(node->left, value);
        return;
    }

    if (node->value < value) {
        removeNode(node->right, value);
        return;
    }

    if (node->left == nullptr) {
        node = std::move(node->right);
        return;
    }

    if (node->right == nullptr) {
        node = std::move(node->left);
        return;
    }

    _removeRecursive(node, node->left);
}

template <std::totally_ordered T>
static Node<T>* search(const std::unique_ptr<Node<T>> &node, const T& value) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->value == value) {
        return node.get();
    } else if (node->value > value) {
        return search(node->left, value);
    } else {
        return search(node->right, value);
    }
}

template <std::totally_ordered T>
static void preOrder(const std::unique_ptr<Node<T>> &node, std::function<void(const T&)> action) {
    if (node == nullptr) {
        return;
    }

    action(node->value);
    preOrder(node->left, action);
    preOrder(node->right, action);
}

template <std::totally_ordered T>
static void inOrder(const std::unique_ptr<Node<T>> &node, std::function<void(const T&)> action) {
    if (node == nullptr) {
        return;
    }

    inOrder(node->left, action);
    action(node->value);
    inOrder(node->right, action);
}

template <std::totally_ordered T>
static void postOrder(const std::unique_ptr<Node<T>> &node, std::function<void(const T&)> action) {
    if (node == nullptr) {
        return;
    }

    postOrder(node->left, action);
    postOrder(node->right, action);
    action(node->value);
}

template <std::totally_ordered T>
static int _treeHeight(Node<T> *node) {
    if (!node) {
        return 0;
    }
    return 1 + std::max(_treeHeight(node->left), _treeHeight(node->right));
}

}  // namespace BinTree
```

= Анализ сложности

== Операция поиска

```cpp
template <std::totally_ordered T>
static Node<T>* search(const std::unique_ptr<Node<T>> &node, const T& value) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->value == value) {
        return node.get();
    } else if (node->value > value) {
        return search(node->left, value);
    } else {
        return search(node->right, value);
    }
}
```

+ Если дерево является *сбалансированным*, то мы можем составить рекуррентное соотношение $T(n) = T(n/2) + O(1)$.
  По теореме о рекурсивном вызове сложность составит $O (log n)$.
+ Если дерево является *вырожденным*, то сложность составит $O (n)$.

#pagebreak()

== Операция добавления

```cpp
template <std::totally_ordered T>
static void addNode(std::unique_ptr<Node<T>> &node, const T& value) {
    if (node == nullptr) {
        node = std::make_unique<Node<T>>(value);
    } else if (node->value > value) {
        addNode(node->left, value);
    } else if (node->value == value) {
        return;
    } else {
        addNode(node->right, value);
    }
}
```

Создание узла выполняется за $O(1)$. Перед этим мы проводим поиск места для вставки.
Алгоритм этого поиска аналогичен поиску, рассмотренному выше, а значит асимтотическая сложность:
+ Для *сбалансированного дерева*: $O(log n)$.
+ Для *вырожденного* дерева: $O(n)$.

#pagebreak()

== Операция удаления

```cpp
template <std::totally_ordered T>
static void _removeRecursive(std::unique_ptr<Node<T>> &a,
                             std::unique_ptr<Node<T>> &b) {
    if (b->right != nullptr) {
        _removeRecursive(a, b->right);
    } else {
        a->value = b->value;
        b = std::move(b->left);
    }
}

template <std::totally_ordered T>
static void removeNode(std::unique_ptr<Node<T>> &node, const T& value) {
    if (node == nullptr) {
        return;
    }

    // 1 случай
    if (node->value > value) {
        removeNode(node->left, value);
        return;
    }

    // 2 случай
    if (node->value < value) {
        removeNode(node->right, value);
        return;
    }

    if (node->left == nullptr) {
        node = std::move(node->right);
        return;
    }

    if (node->right == nullptr) {
        node = std::move(node->left);
        return;
    }

    // 3 случай
    _removeRecursive(node, node->left);
}
```

+ В этом случае у нас происходит поиск элемента и вставка, а значит $O(log n)$ и $O(n)$
  для сбалансированного и вырожденного дерева соответственно.
+ Аналогично первому случаю.
+ В этом случае мы сначала производим поиск ($O(log n$ или $O(n)$, а потом мы начинаем
  идти до самого правого узла в левом поддереве. В сбалансированном дереве в худшем случае
  на этой уйдёт $O(log n)$, а в вырожденном $O(n)$.

  \

  Посчитаем сложность для сбалансированного дерева:
  $ T(n) = T(n/2) + O(log n) = | "по теореме" | = O(log n) $

  Для вырожденного дерева сложность будет $O(n) + O(n) = O(n)$, где первое слагаемое
  --- поиск элемента, а второе --- нахождение самого правого узла в левом поддереве.

*Итого, во всех случаях:*
- Для вырожденного дерева сложность будет $O(n)$.
- Для сбалансированного дерева сложность будет $O(log n)$.

== Операции обхода

Все эти операции выполняются за $O(n)$, так как мы проходимся по всем узлам дерева.
