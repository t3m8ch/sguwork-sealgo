#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>
#include "../src/tree.hpp"

TEST_CASE("Adding black node to root", "[insert]") {
    RedBlackTree<int> tree;
    tree.enableTracing();
    tree.insert(10);

    nlohmann::json expectedJson = {
        {"color", "black"},
        {"value", 10},
        {"left", nullptr},
        {"right", nullptr}
    };
    auto actualJson = tree.toJson();

    tree.saveTrace("add_black_node_to_root_" + std::to_string(std::time(nullptr)) + ".json");

    REQUIRE(expectedJson == actualJson);
}

TEST_CASE("Naive insertion left child", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"color", "black"},
        {"value", 10},
        {"left", nullptr},
        {"right", nullptr}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();
    tree.insert(5);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"color", "black"},
        {"value", 10},
        {"left", {
            {"color", "red"},
            {"value", 5},
            {"left", {}},
            {"right", {}}
        }},
        {"right", nullptr}
    };

    tree.saveTrace("naive_insert_left_child_" + std::to_string(std::time(nullptr)) + ".json");

    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Naive insertion right child", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"color", "black"},
        {"value", 10},
        {"left", nullptr},
        {"right", nullptr}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"color", "black"},
        {"value", 10},
        {"left", nullptr},
        {"right", {
            {"color", "red"},
            {"value", 15},
            {"left", {}},
            {"right", {}}
        }}
    };

    tree.saveTrace("naive_insert_right_child_" + std::to_string(std::time(nullptr)) + ".json");

    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert with red uncle triggers recoloring", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", {
            {"value", 5},
            {"color", "black"},
            {"left", {
                {"value", 3},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 7},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(1);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", {
            {"value", 5},
            {"color", "red"},
            {"left", {
                {"value", 3},
                {"color", "black"},
                {"left", {
                    {"value", 1},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 7},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("insert_with_red_uncle_triggers_recoloring" + std::to_string(std::time(nullptr)) + ".json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert into simple tree with red siblings", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"value", 5},
        {"color", "black"},
        {"left", {
            {"value", 3},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 7},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(1);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 5},
        {"color", "black"},
        {"left", {
            {"value", 3},
            {"color", "black"},
            {"left", {
                {"value", 1},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 7},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("insert_into_simple_tree_with_red_siblings" + std::to_string(std::time(nullptr)) + ".json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert triggering root rotation and restructuring", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", {
            {"value", 5},
            {"color", "red"},
            {"left", {
                {"value", 3},
                {"color", "black"},
                {"left", {
                    {"value", 1},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 2},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }},
            {"right", {
                {"value", 7},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(0);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 5},
        {"color", "black"},
        {"left", {
            {"value", 3},
            {"color", "red"},
            {"left", {
                {"value", 1},
                {"color", "black"},
                {"left", {
                    {"value", 0},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 2},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 10},
            {"color", "red"},
            {"left", {
                {"value", 7},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 15},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }}
    };

    tree.saveTrace("insert_triggering_root_rotation_and_restructuring" + std::to_string(std::time(nullptr)) + ".json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert into complex tree maintaining balance", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", {
            {"value", 5},
            {"color", "red"},
            {"left", {
                {"value", 3},
                {"color", "black"},
                {"left", {
                    {"value", 1},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 2},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }},
            {"right", {
                {"value", 7},
                {"color", "black"},
                {"left", {
                    {"value", 6},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 8},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }}
        }},
        {"right", {
            {"value", 15},
            {"color", "black"},
            {"left", {
                {"value", 14},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 16},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(0);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 5},
        {"color", "black"},
        {"left", {
            {"value", 3},
            {"color", "red"},
            {"left", {
                {"value", 1},
                {"color", "black"},
                {"left", {
                    {"value", 0},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 2},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 10},
            {"color", "red"},
            {"left", {
                {"value", 7},
                {"color", "black"},
                {"left", {
                    {"value", 6},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 8},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }},
            {"right", {
                {"value", 15},
                {"color", "black"},
                {"left", {
                    {"value", 14},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 16},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }}
        }}
    };

    tree.saveTrace("insert_into_complex_tree_maintaining_balance" + std::to_string(std::time(nullptr)) + ".json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert triggering Left-Right case with rotations", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", {
            {"value", 5},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", nullptr}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(7);

    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 7},
        {"color", "black"},
        {"left", {
            {"value", 5},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 10},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("insert_triggering_left_right_case_with_rotations" + std::to_string(std::time(nullptr)) + ".json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert triggering Right-Left case with rotations", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", nullptr},
        {"right", {
            {"value", 15},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(12);

    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 12},
        {"color", "black"},
        {"left", {
            {"value", 10},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 15},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("insert_triggering_right_left_case_with_rotations" + std::to_string(std::time(nullptr)) + ".json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert triggering Right-Right case with rotation", "[insert]") {
    nlohmann::json inputJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", nullptr},
        {"right", {
            {"value", 15},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing();

    tree.insert(20);

    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 15},
        {"color", "black"},
        {"left", {
            {"value", 10},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 20},
            {"color", "red"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("insert_triggering_right_right_case_with_rotations" + std::to_string(std::time(nullptr)) + ".json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}
