add_executable(tests
    test_calc.cpp
    ../src/calc.cpp
)

target_include_directories(tests PRIVATE ../src)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
