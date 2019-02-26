// TODO: 'CATCH_CONFIG_MAIN' keyword should be removed after separating test codes.
#define CATCH_CONFIG_MAIN

#include <vector>

#include "../lib/catch.hpp"
#include "../src/type_converter.hpp"

using namespace std;

TEST_CASE("TypeConverter#integerToBytes") {
  SECTION("Basic case") {
    auto actual = TypeConverter::integerToBytes(1);

    vector<uint8_t> expected {0, 0, 0, 1};

    REQUIRE(actual == expected);
  }

  SECTION("It should work in case that big integer is given") {
    auto actual = TypeConverter::integerToBytes(16772216);

    vector<uint8_t> expected {0, 0xff, 0xec, 0x78};

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter#base64ToArray") {
  SECTION("Basic case") {
    vector<uint8_t> bytes {0x1};

    auto actual = TypeConverter::bytesToArray<1>(bytes);

    array<uint8_t, 1> expected;
    expected.back() = 0x01;

    REQUIRE(actual == expected);
  }

  SECTION("It should work in case that big integer is given") {
    vector<uint8_t> bytes {0xff, 0xec, 0x78};

    auto actual = TypeConverter::bytesToArray<3>(bytes);

    array<uint8_t, 3> expected {0xff, 0xec, 0x78};

    REQUIRE(actual == expected);
  }
}