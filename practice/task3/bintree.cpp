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

template <std::totally_ordered T>
static void print(Node<T>* root) {
    // Установка локали для поддержки Unicode
    std::setlocale(LC_ALL, "");

    if (!root) {
        return;
    }

    // Цветовые коды ANSI
    const std::wstring GREEN_COLOR = L"\033[32m";
    const std::wstring RESET_COLOR = L"\033[0m";

    // Используем wstring вместо string для хранения символов Unicode
    std::map<int, std::map<int, std::wstring>> nodePositions;
    std::map<int, std::map<int, std::pair<int, int>>> connections;

    // Сохраняем позиции и размеры узлов для раскраски
    std::map<int, std::map<int, int>> nodeWidths;

    auto getNodeWidth = [](const T& value) -> int {
        std::ostringstream oss;
        oss << value;
        return oss.str().length();
    };

    int maxDepth = 0;
    int maxWidth = 0;

    std::function<int(Node<T>*, int, int)> computePositions =
        [&](Node<T> *node, int depth, int hPos) -> int {
            if (!node) {
                return hPos;
            }

            maxDepth = std::max(maxDepth, depth);

            int leftPos = computePositions(node->left.get(), depth + 1, hPos);

            std::ostringstream oss;
            oss << node->value;
            std::string nodeStr = oss.str();

            int currPos = (node->left) ? leftPos + 2 : hPos;

            // Конвертируем string в wstring
            std::wstring wNodeStr(nodeStr.begin(), nodeStr.end());
            nodePositions[depth][currPos] = wNodeStr;
            // Сохраняем ширину узла для раскраски
            nodeWidths[depth][currPos] = nodeStr.length();

            maxWidth = std::max(maxWidth, currPos + static_cast<int>(nodeStr.length()));

            if (node->left) {
                for (const auto& [childPos, childStr] : nodePositions[depth + 1]) {
                    if (childPos <= leftPos) {
                        connections[depth][currPos] = std::make_pair(depth + 1, childPos);
                        break;
                    }
                }
            }

            int rightPos = computePositions(node->right.get(), depth + 1, currPos + nodeStr.length() + 1);

            if (node->right) {
                for (const auto& [childPos, childStr] : nodePositions[depth + 1]) {
                    if (childPos >= currPos + static_cast<int>(nodeStr.length())) {
                        connections[depth][currPos + nodeStr.length() - 1] = std::make_pair(depth + 1, childPos);
                        break;
                    }
                }
            }

            return rightPos;
        };

    computePositions(root, 0, 0);

    // Создаем результат без цветов сначала
    std::vector<std::wstring> result(maxDepth * 2 + 1, std::wstring(maxWidth + 1, L' '));

    // Заполняем рёбра
    for (const auto& [fromDepth, connections_map] : connections) {
        for (const auto& [fromPos, toConnection] : connections_map) {
            int toDepth = toConnection.first;
            int toPos = toConnection.second;

            if (toPos < fromPos) {
                result[fromDepth * 2][fromPos - 1] = L'┐';
                result[(fromDepth * 2) + 1][fromPos - 1] = L'│';

                for (int i = fromPos - 2; i > toPos; --i) {
                    result[(fromDepth * 2) + 1][i] = L'─';
                }

                result[(fromDepth * 2) + 1][toPos] = L'┌';
                result[(fromDepth * 2) + 2][toPos] = L'│';
            } else {
                result[fromDepth * 2][fromPos + 1] = L'┌';
                result[(fromDepth * 2) + 1][fromPos + 1] = L'│';

                for (int i = fromPos + 2; i < toPos; ++i) {
                    result[(fromDepth * 2) + 1][i] = L'─';
                }

                result[(fromDepth * 2) + 1][toPos] = L'┐';
                result[(fromDepth * 2) + 2][toPos] = L'│';
            }
        }
    }

    // Выводим результат с раскрашенными узлами
    for (int rowIdx = 0; rowIdx < result.size(); ++rowIdx) {
        for (int colIdx = 0; colIdx < result[rowIdx].length(); ++colIdx) {
            // Проверяем, является ли текущая позиция началом узла
            int depth = rowIdx / 2;
            if (rowIdx % 2 == 0 && nodePositions.count(depth) && nodePositions[depth].count(colIdx)) {
                // Выводим узел зелёным цветом
                std::wcout << GREEN_COLOR << nodePositions[depth][colIdx] << RESET_COLOR;
                // Пропускаем символы, которые занимает значение узла
                colIdx += nodeWidths[depth][colIdx] - 1;
            } else {
                // Выводим символ ребра
                std::wcout << result[rowIdx][colIdx];
            }
        }
        std::wcout << std::endl;
    }
}

}  // namespace BinTree

int main() {
    std::unique_ptr<BinTree::Node<int>> root = nullptr;

    BinTree::addNode(root, 5);
    BinTree::addNode(root, 3);
    BinTree::addNode(root, 7);
    BinTree::addNode(root, 12);

    BinTree::preOrder(root, std::function<void(const int&)>([](int value) {
        std::cout << value << ' ';
    }));
    std::cout << std::endl;

    BinTree::inOrder(root, std::function<void(const int&)>([](int value) {
        std::cout << value << ' ';
    }));
    std::cout << std::endl;

    BinTree::postOrder(root, std::function<void(const int&)>([](int value) {
        std::cout << value << ' ';
    }));
    std::cout << std::endl;

    return 0;
}
