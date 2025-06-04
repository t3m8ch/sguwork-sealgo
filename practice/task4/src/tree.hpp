#pragma once

#include <concepts>
#include <nlohmann/json.hpp>

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
        const T& getValue() const { return value.value(); }
    };

    std::shared_ptr<Node> NIL;
    std::shared_ptr<Node> root;

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
