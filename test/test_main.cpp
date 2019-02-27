#define CATCH_CONFIG_MAIN

#include <vector>

#include "../lib/catch.hpp"
#include "./ecdsa_test.cpp"

#include "./random_number_generator_test.cpp"
#include "./sha256_test.cpp"
#include "./type_converter_test.cpp"

TEST_CASE("All test cases reside in other .cpp files", "[multi-file:1]") {}
