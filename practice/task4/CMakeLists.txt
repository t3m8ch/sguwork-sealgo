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

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-34-2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(asio)

set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)
set(ASIO_INCLUDE_DIRS ${ASIO_INCLUDE_DIR})

add_library(asio_interface INTERFACE)
target_include_directories(asio_interface INTERFACE ${ASIO_INCLUDE_DIR})
target_compile_definitions(asio_interface INTERFACE ASIO_STANDALONE)

FetchContent_Declare(crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.1.2
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(crow)

add_executable(red-black-tree
    src/main.cpp
)

target_link_libraries(red-black-tree
    PRIVATE
    Crow::Crow
    nlohmann_json::nlohmann_json
    pthread
)
target_include_directories(red-black-tree PRIVATE src)

file(COPY ${CMAKE_SOURCE_DIR}/static DESTINATION ${CMAKE_BINARY_DIR})

enable_testing()
add_subdirectory(tests)
