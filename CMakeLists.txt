cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gruut_utils LANGUAGES CXX)
add_subdirectory(test)

add_executable(
        gruut_utils
        main.cpp
)

add_custom_command(
        TARGET gruut_utils
        POST_BUILD
        COMMAND clang-format -i ./src/*.hpp
)
