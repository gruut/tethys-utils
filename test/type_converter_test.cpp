#include <vector>

#include "../lib/catch.hpp"
#include "../src/type_converter.hpp"

using namespace std;

TEST_CASE("TypeConverter.integerToBytes") {
  SECTION("Basic case") {
    auto actual = TypeConverter::integerToBytes(1);

    vector<uint8_t> expected{0, 0, 0, 1};

    REQUIRE(actual == expected);
  }

  SECTION("It should work in case that big integer is given") {
    auto actual = TypeConverter::integerToBytes(16772216);

    vector<uint8_t> expected{0, 0xff, 0xec, 0x78};

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.bytesToArray") {
  SECTION("Basic case") {
    vector<uint8_t> bytes{0x1};

    auto actual = TypeConverter::bytesToArray<1>(bytes);

    array<uint8_t, 1> expected;
    expected.back() = 0x01;

    REQUIRE(actual == expected);
  }

  SECTION("It should work in case that big integer is given") {
    vector<uint8_t> bytes{0xff, 0xec, 0x78};

    auto actual = TypeConverter::bytesToArray<3>(bytes);

    array<uint8_t, 3> expected{0xff, 0xec, 0x78};

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.decodeBase64") {
  SECTION("It should return decoded characters") {
    string_view b64_str = "SGVsbG8="sv;
    auto actual = TypeConverter::decodeBase64(b64_str);

    string_view expected = "Hello"sv;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.base64ToArray") {
  const int array_size = 8;

  SECTION("It should return decoded base64 string") {
    string_view b64_str = "SGVsbG8="sv;
    auto actual = TypeConverter::base64ToArray<array_size>(b64_str);

    auto expected_str = "Hello"s;
    array<uint8_t, array_size> expected;
    std::copy(expected_str.begin(), expected_str.begin() + expected_str.size(), expected.begin());

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.bytesToString") {
  SECTION("It should return string") {
    vector<uint8_t> v = {0x0, 0x0, 0x0, 0x01};
    auto actual = TypeConverter::bytesToString(v);

    auto expected = "\x00\x00\x00\x01"s;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.arrayToString") {
  const int array_size = 4;

  SECTION("It should work") {
    array<uint8_t, array_size> arr = {0x0, 0x0, 0x0, 0x01};
    auto actual = TypeConverter::arrayToString<array_size>(arr);

    auto expected = "\x00\x00\x00\x01"s;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.stringToBytes") {
  SECTION("It should work") {
    auto actual = TypeConverter::stringToBytes("Hello"s);

    auto expected = vector<uint8_t>{'H', 'e', 'l', 'l', 'o'};

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.encodeBase64") {
  SECTION("It should work") {
    auto actual = TypeConverter::encodeBase64("Hello"s);

    auto expected = "SGVsbG8="s;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("TypeConverter.toString") {
  SECTION("It should work") {
    auto actual = TypeConverter::toString("Hello"s);

    auto expected = "Hello"s;

    REQUIRE(actual == expected);
  }
}
