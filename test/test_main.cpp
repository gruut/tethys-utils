#define CATCH_CONFIG_MAIN

#include "../lib/catch.hpp"
#include "./bytes_builder.cpp"
#include "./ecdsa_test.cpp"
#include "./random_number_generator_test.cpp"
#include "./sha256_test.cpp"
#include "./type_converter_test.cpp"
#include "hmac_key_maker_test.cpp"
#include "hmac_test.cpp"
#include "lz4_compressor_test.cpp"
#include "ags_test.cpp"

TEST_CASE("All test cases reside in other .cpp files", "[multi-file:1]") {}
