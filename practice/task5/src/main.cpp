#include "avl-tree.hpp"
#include <iostream>

int main() {
  AVLTree<int32_t> tree;

  while (true) {
    std::string cmd;
    std::cin >> cmd;

    if (cmd == "exit") {
      break;
    }

    if (cmd == "insert") {
      int32_t n;
      std::cin >> n;
      tree.insert(n);
      tree.print();
    }

    if (cmd == "remove") {
      int32_t n;
      std::cin >> n;
      tree.remove(n);
      tree.print();
    }

    if (cmd == "print") {
      tree.print();
    }

    if (cmd == "preorder") {
      tree.preOrder();
    }
  }
}
