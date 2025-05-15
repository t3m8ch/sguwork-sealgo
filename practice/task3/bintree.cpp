#include <concepts>
#include <functional>
#include <memory>

template <class T>
  requires requires(const T &a, const T &b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
  }
class BinaryTree {
private:
  struct Node {
    T value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };

  using OrderFn = std::function<void(const T)>;
  using NodePtr = std::unique_ptr<Node>;

  void add(std::unique_ptr<Node> &node, T value) {
    if (node == nullptr) {
      node = std::make_unique<Node>(value);
    } else if (node->value > value) {
      add(node->left, value);
    } else {
      add(node->right, value);
    }
  }

  void preOrder(NodePtr &node, OrderFn fn) {
    if (node == nullptr) {
      return;
    }

    fn(node->value);
    preOrder(node->left, fn);
    preOrder(node->right, fn);
  }

  void inOrder(NodePtr &node, OrderFn fn) {
    if (node == nullptr) {
      return;
    }

    inOrder(node->left, fn);
    fn(node->value);
    inOrder(node->right, fn);
  }

  void postOrder(NodePtr &node, OrderFn fn) {
    if (node == nullptr) {
      return;
    }

    postOrder(node->left, fn);
    postOrder(node->right, fn);
    fn(node->value);
  }

  NodePtr search(const NodePtr &node, T value) {
    if (node == nullptr) {
      return nullptr;
    }

    if (node->value > value) {
      return search(node->left, value);
    } else if (node->value < value) {
      return search(node->right, value);
    } else {
      return node;
    }
  }

  bool remove(NodePtr &node, T value) {
    if (node == nullptr) {
      return false;
    }

    if (node->value > value) {
      return remove(node->left, value);
    }

    if (node->left == nullptr) {
      node = node->right;
    } else if (node->right == nullptr) {
      node = node->left;
    } else {
      removeRecursive(node, value);
    }

    return true;
  }

  void removeRecursive(const NodePtr &node, NodePtr &subTree) {
    if (node->right != nullptr) {
      removeRecursive(node, subTree->right);
    } else {
      node->value = subTree->value;
      subTree = subTree->left;
    }
  }

  NodePtr tree;
  BinaryTree<T>(NodePtr &node) { this->tree = node; }

public:
  BinaryTree<T>() { this->tree = nullptr; }

  void add(T value) { add(this->tree, value); }
  void preOrder(OrderFn fn) { preOrder(this->tree, fn); }
  void inOrder(OrderFn fn) { inOrder(this->tree, fn); }
  void postOrder(OrderFn fn) { postOrder(this->tree, fn); }
  bool remove(T value) { return remove(this->tree, value); }

  BinaryTree<T> search(T value) {
    return BinaryTree<T>(search(this->tree, value));
  }
};
