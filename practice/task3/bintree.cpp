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
    } else if (node.value > value) {
      add(node, value);
    }
  }

  void preOrder(NodePtr &node, OrderFn fn) {
    if (node == nullptr) {
      return;
    }

    fn(node.value);
    preOrder(node.left);
    preOrder(node.right);
  }

  void inOreder(NodePtr &node, OrderFn fn) {
    if (node == nullptr) {
      return;
    }

    inOrder(node.left);
    fn(node.value);
    inOrder(node.right);
  }

  void postOrder(NodePtr &node, OrderFn fn) {
    if (node == nullptr) {
      return;
    }

    postOrder(node.left);
    postOrder(node.right);
    fn(node.value);
  }
}
