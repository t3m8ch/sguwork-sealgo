#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>
#include "../src/tree.hpp"

auto currentTime = std::to_string(std::time(nullptr));

TEST_CASE("Adding black node to root", "[insert]") {
    RedBlackTree<int> tree;
    tree.enableTracing(currentTime);
    tree.insert(10);

    nlohmann::json expectedJson = {
        {"color", "black"},
        {"value", 10},
        {"left", nullptr},
        {"right", nullptr}
    };
    auto actualJson = tree.toJson();

    tree.saveTrace("add_black_node_to_root.json");

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
    tree.enableTracing(currentTime);
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

    tree.saveTrace("naive_insert_left_child.json");

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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("naive_insert_right_child.json");

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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("insert_with_red_uncle_triggers_recoloring.json");
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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("insert_into_simple_tree_with_red_siblings.json");
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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("insert_triggering_root_rotation_and_restructuring.json");
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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("insert_into_complex_tree_maintaining_balance.json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Insert triggering Left-Left case with rotations", "[insert]") {
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
    tree.enableTracing(currentTime);

    tree.insert(3);

    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 5},
        {"color", "black"},
        {"left", {
            {"value", 3},
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

    tree.saveTrace("insert_triggering_left_left_case_with_rotations.json");
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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("insert_triggering_left_right_case_with_rotations.json");
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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("insert_triggering_right_left_case_with_rotations.json");
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
    tree.enableTracing(currentTime);

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

    tree.saveTrace("insert_triggering_right_right_case_with_rotations.json");
    REQUIRE(actualJsonTree == expectedJsonTree);
}

TEST_CASE("Remove missing node", "[remove]") {
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
    tree.enableTracing(currentTime);

    bool removed = tree.remove(100);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
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

    tree.saveTrace("remove_missing_node.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(!removed);
}

TEST_CASE("Remove red node without children", "[remove]") {
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
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", nullptr},
        {"right", nullptr}
    };

    tree.saveTrace("remove_red_node_without_children.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

// TODO: Create test for black max node in left subtree
TEST_CASE("Remove red node with two children and red max node in left subtree", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "red"},
            {"left", {
                {"value", 10},
                {"color", "black"},
                {"left", {
                    {"value", 5},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 11},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }},
            {"right", {
                {"value", 17},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 10},
            {"color", "red"},
            {"left", {
                {"value", 5},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 17},
                {"color", "black"},
                {"left", {
                    {"value", 11},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_red_node_with_two_children_and_red_max_node_in_left_subtree.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node with one left red child", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", {
                {"value", 10},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 10},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_one_left_red_child.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node with one right red child", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", {
                {"value", 17},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }},
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 17},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_one_right_red_child.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node with black parent, black brother and left red nephew", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", {
                {"value", 23},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 27},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 25},
        {"color", "black"},
        {"left", {
            {"value", 20},
            {"color", "black"},
            {"left", nullptr},
            {"right", {
                {"value", 23},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 27},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_black_parent_black_brother_and_left_red_nephew.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node with black parent, black brother and left black nephew", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", {
                {"value", 23},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 27},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 25},
        {"color", "black"},
        {"left", {
            {"value", 20},
            {"color", "black"},
            {"left", nullptr},
            {"right", {
                {"value", 23},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 27},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_black_parent_black_brother_and_left_black_nephew.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node with red parent, black brother and left red nephew", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 100},
        {"color", "black"},
        {"left", {
            {"value", 20},
            {"color", "red"},
            {"left", {
                {"value", 15},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 25},
                {"color", "black"},
                {"left", {
                    {"value", 23},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 27},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }}
        }},
        {"right", {
            {"value", 150},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 100},
        {"color", "black"},
        {"left", {
            {"value", 25},
            {"color", "red"},
            {"left", {
                {"value", 20},
                {"color", "black"},
                {"left", nullptr},
                {"right", {
                    {"value", 23},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }},
            {"right", {
                {"value", 27},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 150},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_red_parent_black_brother_and_left_red_nephew.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node with red parent, black brother and left black nephew", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 100},
        {"color", "black"},
        {"left", {
            {"value", 20},
            {"color", "red"},
            {"left", {
                {"value", 15},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 25},
                {"color", "black"},
                {"left", {
                    {"value", 23},
                    {"color", "black"},
                    {"left", nullptr},
                    {"right", nullptr}
                }},
                {"right", {
                    {"value", 27},
                    {"color", "red"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }}
        }},
        {"right", {
            {"value", 150},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 100},
        {"color", "black"},
        {"left", {
            {"value", 25},
            {"color", "red"},
            {"left", {
                {"value", 20},
                {"color", "black"},
                {"left", nullptr},
                {"right", {
                    {"value", 23},
                    {"color", "black"},
                    {"left", nullptr},
                    {"right", nullptr}
                }}
            }},
            {"right", {
                {"value", 27},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 150},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_red_parent_black_brother_and_left_black_nephew.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node with black brother, right black (nil) nephew and left red nephew", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", {
                {"value", 23},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(15);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 23},
        {"color", "black"},
        {"left", {
            {"value", 20},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_black_brother_right_black_nephew_and_left_red_nephew.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node, with black root parent and black brother", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 10},
        {"color", "black"},
        {"left", {
            {"value", 5},
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
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(5);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
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

    tree.saveTrace("remove_black_node_without_children_with_black_parent_and_black_brother.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node, with red non-root parent and black brother", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "red"},
            {"left", {
                {"value", 10},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 17},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(10);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", {
                {"value", 17},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", nullptr},
            {"right", nullptr}
        }}
    };

    tree.saveTrace("remove_black_node_with_red_non_root_parent_and_black_brother.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}

TEST_CASE("Remove black node, with black non-root parent and black brother", "[remove]") {
    nlohmann::json inputJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", {
                {"value", 10},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 17},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 25},
            {"color", "black"},
            {"left", {
                {"value", 22},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 30},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }}
    };

    RedBlackTree<int> tree(inputJsonTree);
    tree.enableTracing(currentTime);

    bool removed = tree.remove(10);
    auto actualJsonTree = tree.toJson();

    nlohmann::json expectedJsonTree = {
        {"value", 20},
        {"color", "black"},
        {"left", {
            {"value", 15},
            {"color", "black"},
            {"left", nullptr},
            {"right", {
                {"value", 17},
                {"color", "red"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }},
        {"right", {
            {"value", 25},
            {"color", "red"},
            {"left", {
                {"value", 22},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }},
            {"right", {
                {"value", 30},
                {"color", "black"},
                {"left", nullptr},
                {"right", nullptr}
            }}
        }}
    };

    tree.saveTrace("remove_black_node_with_black_non_root_parent_and_black_brother.json");

    REQUIRE(actualJsonTree == expectedJsonTree);
    REQUIRE(removed);
}
