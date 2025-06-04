#pragma once

#include <concepts>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>

template <std::totally_ordered T>
class RedBlackTree {
public:
    RedBlackTree() {
        NIL = std::make_shared<Node>();
        root = NIL;
    };

    RedBlackTree(const nlohmann::json& json) {
        NIL = std::make_shared<Node>();
        root = fromJson(json, nullptr);
    }

    void insert(const T& value) {
        // Если дерево пустое, создаём корень
        if (root->isNil()) {
            // В КЧД корень обязательно чёрный
            root = std::make_shared<Node>(value, Node::Color::BLACK);

            // Задаём фиктивных детей
            root->left = NIL;
            root->right = NIL;

            // Задаём пустой указатель в качестве родителя
            root->parent = std::weak_ptr<Node>();

            return;
        }

        // Если же дерево не пустое, то мы создаём новый красный узел
        auto node = std::make_shared<Node>(value, Node::Color::RED);
        node->left = NIL;
        node->right = NIL;

        std::shared_ptr<Node> p = root;
        std::shared_ptr<Node> q = NIL;

        // Спускаемся вниз, пока не дойдём до подходящего листа
        while (!p->isNil()) {
            q = p;
            if (*(p->value) < value) {
                p = p->right;
            } else {
                p = p->left;
            }
        }
        node->parent = q;

        // Добавляем новый узел красного цвета
        if (*(q->value) < value) {
            q->right = node;
        } else {
            q->left = node;
        }

        fixAfterInsert(node);
    }

    bool remove(const T& value) {
        // Implementation of the remove function
        return false;
    }

    nlohmann::json toJson() const {
        return toJson(root);
    }
private:
    struct Node {
        enum Color { RED, BLACK };

        std::optional<T> value;
        Color color;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(const T& value, Color color = RED) : value(value), color(color) {}
        Node() : color(BLACK) {}

        bool isNil() const { return !value.has_value(); }
        bool isRoot() const { return parent.expired(); }
        const T& getValue() const { return value.value(); }
    };

    std::shared_ptr<Node> NIL;
    std::shared_ptr<Node> root;

    // Метод, который восстанавливает свойства красно-чёрного дерева после вставки узла
    void fixAfterInsert(std::shared_ptr<Node> node) {
        // Если узел является корнем
        if (node->isRoot()) {
            // то мы его должны перекрасить в чёрный цвет
            node->color = Node::BLACK;
            return;
        }

        while (true) {
            auto parent = node->parent.lock();
            if (!parent || parent->color == Node::BLACK) {
                // Если наш отец чёрный, то мы не нарушаем никаких правил
                // => мы можем закончить наш цикл, КЧД исправлено
                break;
            }

            auto grandfather = parent->parent.lock();
            if (!grandfather) {
                break;
            }

            if (grandfather->left == parent) {
                // Если родитель -- левый ребёнок дедушки,
                // то дядя -- это правый рёбёнок дедушки
                auto uncle = grandfather->right;

                // Если у нас есть красный дядя, то начинаем перекрашивание
                if (!uncle->isNil() && uncle->color == Node::Color::RED) {
                    parent->color = Node::Color::BLACK;
                    uncle->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    // ОСТОРОЖНО
                    node = grandfather;
                } else {
                    // Если же наш дядя чёрный, то начинаем повороты

                    // Если node -- правый сын
                    if (node == parent->right) {
                        // то мы поворачиваем влево
                        // ОСТОРОЖНО
                        node = parent;
                        leftRotate(node);
                    }

                    parent->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    rightRotate(grandfather);
                }
            } else {
                // Если родитель -- правый ребёнок дедушки,
                // то дядя -- это левый рёбёнок дедушки

                auto uncle = grandfather->left;

                // Если у нас есть красный дядя, то начинаем перекрашивание
                if (!uncle->isNil() && uncle->color == Node::Color::RED) {
                    parent->color = Node::Color::BLACK;
                    uncle->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    // ОСТОРОЖНО
                    node = grandfather;
                } else {
                    // Если же наш дядя чёрный, то начинаем повороты

                    // Если node -- левый сын
                    if (node == parent->right) {
                        // то мы поворачиваем влево

                        // ОСТОРОЖНО
                        node = parent;
                        rightRotate(node);
                    }

                    parent->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    leftRotate(grandfather);
                }
            }
        }

        // Красим корень в чёрный
        root->color = Node::Color::BLACK;
    }

    void leftRotate(std::shared_ptr<Node> x) {
        // Правый ребёнок становится новым корнем поддерева
        auto y = x->right;

        // Левый ребёнок y становится правым ребёнком x
        x->right = y->left;
        if (y->left && !y->left->isNil()) {
            y->left->parent = x; // Обновляем родителя левого ребёнка y
        }

        // y занимает место x
        y->parent = x->parent;

        // Обновляем родителя x
        if (auto parent = x->parent.lock()) {
            if (x == parent->left) {
                parent->left = y;
            } else {
                parent->right = y;
            }
        } else {
            // Если мы не смогли найти родителя x, значит x был корнем
            root = y;
        }

        // Делаем x левым ребёнком y
        y->left = x;
        x->parent = y;
    }

    void rightRotate(std::shared_ptr<Node> x) {
        // Левый ребёнок становится новым корнем поддерева
        auto y = x->left;

        // Правый ребёнок y становится левым ребёнком x
        x->left = y->right;
        if (y->right && !y->right->isNil()) {
            y->right->parent = x; // Обновляем родителя правого ребёнка y
        }

        // y занимает место x
        y->parent = x->parent;

        // Обновляем родителя x
        if (auto parent = x->parent.lock()) {
            if (x == parent->right) {
                parent->right = y;
            } else {
                parent->left = y;
            }
        } else {
            // Если мы не смогли найти родителя x, значит x был корнем
            root = y;
        }

        // Делаем y правым ребёнком x
        y->right = x;
        x->parent = y;
    }

    nlohmann::json toJson(std::shared_ptr<Node> node) const {
        if (node->isNil()) {
            return nullptr;
        }

        nlohmann::json json;
        json["value"] = node->getValue();
        json["color"] = (node->color == Node::RED) ? "red" : "black";
        json["left"] = toJson(node->left);
        json["right"] = toJson(node->right);

        return json;
    }

    std::shared_ptr<Node> fromJson(const nlohmann::json& json, std::shared_ptr<Node> parent) {
        if (json.is_null()) {
            return NIL;
        }

        T value = json["value"];
        std::string colorStr = json["color"];
        typename Node::Color color = (colorStr == "red") ? Node::RED : Node::BLACK;

        auto node = std::make_shared<Node>(value, color);
        node->parent = parent;
        node->left = fromJson(json["left"], node);
        node->right = fromJson(json["right"], node);

        return node;
    }
};
