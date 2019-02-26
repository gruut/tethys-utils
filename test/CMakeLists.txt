cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

file(GLOB HEADER_FILES
        "../src/type_converter.hpp"
        )

add_executable(gruut_utils_test test_main.cpp)

target_sources(gruut_utils_test
        PUBLIC
        ${HEADER_FILES}
        )
