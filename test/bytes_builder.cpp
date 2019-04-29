#include "../src/bytes_builder.hpp"
#include "../lib/catch.hpp"

using namespace std;

TEST_CASE("BytesBuilder") {
  SECTION("BytesBuilder append and get") {
    string test_normal_str = "Hello world";   // 11-bytes;
    string test_b64_str = "YmFzZTY0dGVzdA=="; // 10-bytes "base64test"
    string test_b58_str = "6XaWvmhVhb7vJP";   // 10-bytes "base58test"
    string test_hex_str = "61626364";         // 4-bytes
    string test_dec_str = "1234";             // 8-bytes
    uint64_t test_dec_uint64 = 1234;          // 8-bytes
    int test_dec_int = 5678;                  // 4-bytes

    string actual_str = test_normal_str + "base64test" + "base58test";

    actual_str.push_back(0x61);
    actual_str.push_back(0x62);
    actual_str.push_back(0x63);
    actual_str.push_back(0x64);

    uint64_t dec = stoull(test_dec_str);
    for (int i = 7; i >= 0; i--) {
      actual_str.push_back((dec >> (i * 8)) & 0xFF);
    }

    uint64_t uint64_dec = test_dec_uint64;
    for (int i = 7; i >= 0; i--) {
      actual_str.push_back((uint64_dec >> (i * 8)) & 0xFF);
    }

    int int_dec = test_dec_int;
    for (int i = 3; i >= 0; i--) {
      actual_str.push_back((test_dec_int >> (i * 8)) & 0xFF);
    }

    BytesBuilder bytes_builder;
    bytes_builder.append(test_normal_str);
    bytes_builder.appendBase<64>(test_b64_str);
    bytes_builder.appendBase<58>(test_b58_str);
    bytes_builder.appendHex(test_hex_str);
    bytes_builder.appendDec(test_dec_str);
    bytes_builder.appendDec(test_dec_uint64);
    bytes_builder.appendDec(test_dec_int);

    REQUIRE(bytes_builder.getBytes().size() == actual_str.length());
    REQUIRE(bytes_builder.getString() == actual_str);
  }
}
