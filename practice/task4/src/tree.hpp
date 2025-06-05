#pragma once

#include <concepts>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <fstream>

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
        recordStep("insert", "Starting insertion of value: " + std::to_string(value));

        // Если дерево пустое, создаём корень
        if (root->isNil()) {
            recordStep("insert", "Tree is empty, creating root");

            // В КЧД корень обязательно чёрный
            root = std::make_shared<Node>(value, Node::Color::BLACK);

            // Задаём фиктивных детей
            root->left = NIL;
            root->right = NIL;

            // Задаём пустой указатель в качестве родителя
            root->parent = std::weak_ptr<Node>();

            recordStep("insert", "Root created and colored black", root);

            return;
        }

        // Если же дерево не пустое, то мы создаём новый красный узел
        auto node = std::make_shared<Node>(value, Node::Color::RED);
        node->left = NIL;
        node->right = NIL;

        recordStep("insert", "Created new red node: " + std::to_string(value), node);

        std::shared_ptr<Node> p = root;
        std::shared_ptr<Node> q = NIL;

        recordStep("insert", "Searching for insertion position");

        // Спускаемся вниз, пока не дойдём до подходящего листа
        while (!p->isNil()) {
            q = p;
            if (*(p->value) < value) {
                p = p->right;
                recordStep("insert", "Going right from node: " + std::to_string(*(q->value)));
            } else {
                p = p->left;
                recordStep("insert", "Going left from node: " + std::to_string(*(q->value)));
            }
        }
        node->parent = q;

        // Добавляем новый узел красного цвета
        if (*(q->value) < value) {
            q->right = node;
            recordStep("insert", "Inserted as right child of: " + std::to_string(*(q->value)), node);
        } else {
            q->left = node;
            recordStep("insert", "Inserted as left child of: " + std::to_string(*(q->value)), node);
        }

        recordStep("insert", "Starting tree balancing");
        fixAfterInsert(node);
        recordStep("insert", "Insertion completed");
    }

    bool remove(const T& value) {
        // Implementation of the remove function
        return false;
    }

    nlohmann::json toJson() const {
        return toJson(root);
    }

    void enableTracing(const std::string& dir) {
        tracing_enabled = true;
        tracingDir = dir;
        trace_steps.clear();
    }

    void disableTracing() {
        tracing_enabled = false;
    }

    void saveTrace(const std::string& filename) {
        if (trace_steps.empty()) return;

        nlohmann::json trace_json;
        trace_json["steps"] = trace_steps;
        trace_json["total_steps"] = trace_steps.size();

        std::string filenameWithDir = "tracing/" + tracingDir + "/" + filename;
        std::filesystem::path path(filenameWithDir);
        std::filesystem::create_directories(path.parent_path());

        std::ofstream file(filenameWithDir);
        file << trace_json.dump(2);
        file.close();
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
            recordStep("fix", "Node is root, coloring black", node);

            // то мы его должны перекрасить в чёрный цвет
            node->color = Node::BLACK;
            return;
        }

        while (true) {
            auto parent = node->parent.lock();
            if (!parent || parent->color == Node::BLACK) {
                recordStep("fix", "Parent is black or null, balancing complete", node);

                // Если наш отец чёрный, то мы не нарушаем никаких правил
                // => мы можем закончить наш цикл, КЧД исправлено
                break;
            }

            recordStep("fix", "Parent is red, need to fix", parent);

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
                    recordStep("fix", "Red uncle case - recoloring", uncle);

                    parent->color = Node::Color::BLACK;
                    uncle->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    recordStep("fix", "Recolored parent, uncle, grandfather");

                    // ОСТОРОЖНО
                    node = grandfather;
                } else {
                    recordStep("fix", "Black uncle case - rotations needed");

                    // Если же наш дядя чёрный, то начинаем повороты

                    // Если node -- правый сын
                    if (node == parent->right) {
                        recordStep("fix", "Left rotation needed", node);

                        // то мы поворачиваем влево
                        node = parent;
                        leftRotate(node);

                        recordStep("fix", "Left rotation completed");
                    }

                    recordStep("fix", "Parent and grandfather recoloring needed");

                    parent = node->parent.lock();
                    grandfather = parent->parent.lock();

                    parent->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;
                    recordStep("fix", "Parent and grandfather recoloring completed");

                    recordStep("fix", "Right rotation needed", grandfather);
                    rightRotate(grandfather);
                    recordStep("fix", "Right rotation completed");
                }
            } else {
                // Если родитель -- правый ребёнок дедушки,
                // то дядя -- это левый рёбёнок дедушки

                auto uncle = grandfather->left;

                // Если у нас есть красный дядя, то начинаем перекрашивание
                if (!uncle->isNil() && uncle->color == Node::Color::RED) {
                    recordStep("fix", "Red uncle case - recoloring", uncle);

                    parent->color = Node::Color::BLACK;
                    uncle->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    recordStep("fix", "Recolored parent, uncle, grandfather");
                    // ОСТОРОЖНО
                    node = grandfather;
                } else {
                    recordStep("fix", "Black uncle case - rotations needed");

                    // Если же наш дядя чёрный, то начинаем повороты

                    // Если node -- левый сын
                    if (node == parent->left) {
                        // то мы поворачиваем влево

                        recordStep("fix", "Right rotation needed", node);

                        node = parent;
                        rightRotate(node);

                        recordStep("fix", "Right rotation completed");
                    }

                    recordStep("fix", "Parent and grandfather recoloring needed");

                    parent = node->parent.lock();
                    grandfather = parent->parent.lock();
                    parent->color = Node::Color::BLACK;
                    grandfather->color = Node::Color::RED;

                    recordStep("fix", "Parent and grandfather recoloring completed");

                    recordStep("fix", "Left rotation needed", grandfather);
                    leftRotate(grandfather);
                    recordStep("fix", "Left rotation completed");
                }
            }
        }

        // Красим корень в чёрный
        root->color = Node::Color::BLACK;
    }

    void leftRotate(std::shared_ptr<Node> x) {
        // Правый ребёнок становится новым корнем поддерева
        auto y = x->right;
        recordStep("leftRotate", "auto y = x->right");

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
        recordStep("leftRotate", "Update x parent");

        // Делаем x левым ребёнком y
        y->left = x;
        x->parent = y;
        recordStep("leftRotate", "x->left = x and x->parent = y");
    }

    void rightRotate(std::shared_ptr<Node> x) {
        // Левый ребёнок становится новым корнем поддерева
        auto y = x->left;
        recordStep("rightRotate", "y = x->left");

        // Правый ребёнок y становится левым ребёнком x
        x->left = y->right;
        if (y->right && !y->right->isNil()) {
            y->right->parent = x; // Обновляем родителя правого ребёнка y
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
        recordStep("rightRotate", "Update x parent");

        // Делаем y правым ребёнком x
        y->right = x;
        x->parent = y;
        recordStep("rightRotate", "x->right = x and x->parent = y");
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

    bool tracing_enabled = false;
    std::vector<nlohmann::json> trace_steps;
    std::string current_operation;
    std::string tracingDir;

    void recordStep(const std::string& operation, const std::string& description,
                   std::shared_ptr<Node> highlighted_node = nullptr) {
        if (!tracing_enabled) return;

        nlohmann::json step;
        step["operation"] = operation;
        step["description"] = description;
        step["tree_state"] = toJson();
        step["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

        if (highlighted_node && !highlighted_node->isNil()) {
            step["highlighted_node"] = highlighted_node->getValue();
        }

        trace_steps.push_back(step);
    }
};
