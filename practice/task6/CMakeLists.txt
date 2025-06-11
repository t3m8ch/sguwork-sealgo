cmake_minimum_required(VERSION 3.14)
project(strings VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET FALSE)

add_executable(functions
    cmd/functions.cpp
)
target_include_directories(functions PRIVATE internal)

add_executable(kmp
    cmd/kmp-search.cpp
)
target_include_directories(kmp PRIVATE internal)

add_executable(bmh
    cmd/bmh-search.cpp
)
target_include_directories(bmh PRIVATE internal)
