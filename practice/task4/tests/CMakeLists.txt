add_executable(tests
    test_tree.cpp
)

target_include_directories(tests PRIVATE ../src)
target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
