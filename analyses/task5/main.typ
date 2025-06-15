#import "../conf.typ": conf

#show: conf.with(
  meta: (
    title: "Анализ АВЛ-дерева",
    author: "Кудяков Артём Александрович",
    group: "251",
    city: "Саратов",
    year: 2025,
  )
)

= Код программы

```cpp
#include <concepts>
#include <iostream>
#include <memory>

template <std::totally_ordered T> class AVLTree {
public:
  AVLTree() : root(nullptr) {}

  void insert(const T &value) {
    if (root == nullptr) {
      root = std::make_shared<Node>(value);
    } else {
      insert(root, value);
    }
  }
  void remove(const T &value) { remove(root, value); }

  void preOrder() {
    preOrder(root);
    std::cout << std::endl;
  }

  void print() { print(root, 0); }

private:
  struct Node {
    T value;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    // Высота узла - длина максимального пути от узла до самого нижнего листа
    // Листовой узел имеет высоту 0. Несуществующий узел имеет высоту -1.
    int32_t height;

    Node(const T &value)
        : value(value), left(nullptr), right(nullptr), height(0) {}
  };

  std::shared_ptr<Node> root;

  void insert(std::shared_ptr<Node> node, const T &value) {
    if (value < node->value) {
      if (node->left == nullptr) {
        node->left = std::make_shared<Node>(value);
      } else {
        insert(node->left, value);
      }
    } else if (value >= node->value) {
      if (node->right == nullptr) {
        node->right = std::make_shared<Node>(value);
      } else {
        insert(node->right, value);
      }
    }

    updateHeight(node);
    balance(node);
  }

  std::shared_ptr<Node> remove(std::shared_ptr<Node> &node, const T &value) {
    if (node == nullptr) {
      return nullptr;
    } else if (value < node->value) {
      node->left = remove(node->left, value);
    } else if (value > node->value) {
      node->right = remove(node->right, value);
    } else if (node->left == nullptr) {
      node = node->right;
    } else if (node->right == nullptr) {
      node = node->left;
    } else {
      auto maxInLeft = getMax(node->left);
      node->value = maxInLeft->value;
      node->right = remove(node->right, maxInLeft->value);
    }

    if (node != nullptr) {
      updateHeight(node);
      balance(node);
    }

    return node;
  }

  void preOrder(std::shared_ptr<Node> node) {
    if (node == nullptr) {
      return;
    }

    std::cout << (node->value) << '(' << getHeight(node) << ") ";
    preOrder(node->left);
    preOrder(node->right);
  }

  void print(std::shared_ptr<Node> node, int32_t depth) {
    if (node == nullptr) {
      return;
    }

    for (int i = 0; i < depth; i++) {
      std::cout << '-';
    }

    if (depth != 0) {
      std::cout << ' ';
    }

    std::cout << (node->value) << '(' << getHeight(node) << ") " << std::endl;
    print(node->left, depth + 1);
    print(node->right, depth + 1);
  }

  int32_t getHeight(std::shared_ptr<Node> node) {
    if (node == nullptr) {
      return -1;
    }

    return node->height;
  }

  void updateHeight(std::shared_ptr<Node> node) {
    if (node == nullptr) {
      return;
    }

    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
  }

  // Если разница между высотами левого и правого поддерева по модулю больше 1,
  // это значит, что пора балансировать дерево. Если число отрицательное,
  // значит дерево перегружено в левую сторону, а значит надо балансировать
  // вправо; в противном случае дерево перегружено в правую сторону, а значит
  // надо балансировать влево.
  int32_t getBalance(std::shared_ptr<Node> node) {
    if (node == nullptr) {
      return 0;
    }

    return getHeight(node->right) - getHeight(node->left);
  }

  void rightRotate(std::shared_ptr<Node> node) {
    // В случае, если у нас нет левого потомка,
    // мы не можем произвести правый поворот
    if (node == nullptr || node->left == nullptr) {
      return;
    }

    std::swap(node->value, node->left->value);
    auto buf = node->right;
    node->right = node->left;
    node->left = node->right->left;
    node->right->left = node->right->right;
    node->right->right = buf;

    // После поворота необходимо обновить высоты
    updateHeight(node->right);
    updateHeight(node);
  }

  void leftRotate(std::shared_ptr<Node> node) {
    // В случае, если у нас нет правого потомка,
    // мы не можем произвести левый поворот
    if (node == nullptr || node->right == nullptr) {
      return;
    }

    std::swap(node->value, node->right->value);
    auto buf = node->left;
    node->left = node->right;
    node->right = node->left->right;
    node->left->right = node->left->left;
    node->left->left = buf;

    updateHeight(node->left);
    updateHeight(node);
  }

  void balance(std::shared_ptr<Node> node) {
    int32_t balance = getBalance(node);
    if (balance == -2) {
      if (getBalance(node->left) == 1) {
        leftRotate(node->left);
      }
      rightRotate(node);
    } else if (balance == 2) {
      if (getBalance(node->right) == -1) {
        rightRotate(node->right);
      }
      leftRotate(node);
    }
  }

  std::shared_ptr<Node> getMax(std::shared_ptr<Node> node) {
    if (node == nullptr || node->right == nullptr) {
      return node;
    }

    return getMax(node->right);
  }
};
```

#pagebreak()

= Анализ сложности

== Высота дерева

Функция `getHeight` достаёт высоту из узла дерева, это занимает $O(1)$.

== Получение баланса

Функция `getBalance` сравнивает высоты корней левого и правого поддеревьев, это занимает $O(1) + O(1) = O(1)$.

== Повороты

Каждый поворот выполняется за $O(1)$. В худшем случае в дереве происходит не более 2 поворотов, что также занимает $O(1)$.

== Вставка

+ Сначала происходит поиск узла, что занимает $O(log n)$, так как АВЛ-дерево сбалансированное.
+ Потом происходит балансировка. В худшем случае для этого необходимо произвести два поворота --- $O(1)$.

*Итоговая сложность вставки $O(log n) + O(1) = O(log n)$*.

== Удаление

+ Сначала происходит поиск узла, что занимает $O(log n)$, так как АВЛ-дерево сбалансированное.
+ Потом мы производим удаление узла:
  + Если у узла нет потомков --- $O(1)$.
  + Если у узла один потомок --- $O(1)$, ведь в таком случае мы просто заменяем узел на его потомка.
  + Если у узла два потомка --- $O(log n)$, так как мы ищем минимальный узел в правом поддереве.

== Обходы

Занимают $O(n)$, так как мы проходим по всем узлам дерева.
