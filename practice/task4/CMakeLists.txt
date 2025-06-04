cmake_minimum_required(VERSION 3.14)
project(red-black-tree VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
Set(FETCHCONTENT_QUIET FALSE)

include(FetchContent)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(json)

add_executable(red-black-tree
    src/main.cpp
)

target_link_libraries(red-black-tree PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(red-black-tree PRIVATE src)

enable_testing()
add_subdirectory(tests)
