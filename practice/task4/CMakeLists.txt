cmake_minimum_required(VERSION 3.14)
project(red-black-tree VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(red-black-tree
    src/main.cpp
    src/calc.cpp
)

target_include_directories(red-black-tree PRIVATE src)

enable_testing()
add_subdirectory(tests)
