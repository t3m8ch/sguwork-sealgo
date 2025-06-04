#pragma once

#include <concepts>
#include <nlohmann/json.hpp>

template <std::totally_ordered T>
class RedBlackTree {
public:
    RedBlackTree() {
    };

    RedBlackTree(const nlohmann::json& json) {
    }

    void insert(const T& value) {
    }

    bool remove(const T& value) {
    }

    nlohmann::json toJson() const {
    }
private:
};
