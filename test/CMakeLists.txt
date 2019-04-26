cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

set(LIB_PREFIX "/usr/local/lib")
set(BOTAN_LIBS "${LIB_PREFIX}/libbotan-2.a")
set(LZ4_LIBS "${LIB_PREFIX}/liblz4.a")

file(GLOB HEADER_FILES
        "../src/*.hpp"
        )

file (GLOB SRC_FILES
        "../src/*.cpp"
        )

add_executable(gruut_utils_test test_main.cpp)

target_sources(gruut_utils_test
        PUBLIC
        ${HEADER_FILES}
        ${SRC_FILES}
        )

target_link_libraries(gruut_utils_test
        PRIVATE
        ${BOTAN_LIBS}
        ${LZ4_LIBS}
        )

IF (NOT $ENV{TRAVIS})
    add_custom_command(
            TARGET gruut_utils_test
            POST_BUILD
            COMMAND clang-format -i *.cpp
    )
ENDIF ()