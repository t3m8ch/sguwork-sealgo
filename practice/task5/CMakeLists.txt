cmake_minimum_required(VERSION 3.14)
project(avl-tree VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
Set(FETCHCONTENT_QUIET FALSE)

add_executable(avl-tree
    src/main.cpp
)

target_include_directories(avl-tree PRIVATE src)
