cmake_minimum_required(VERSION 3.14)
project(prefix-z-function VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET FALSE)

add_executable(prefix-z-function
    src/main.cpp
)

target_include_directories(prefix-z-function PRIVATE src)
