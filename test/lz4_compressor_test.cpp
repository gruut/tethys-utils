#include <vector>

#include "../lib/catch.hpp"
#include "../src/lz4_compressor.hpp"

using namespace std;

TEST_CASE("LZ4Compressor.compressData") {
  string_view msg = "Hello world"sv;

  SECTION("It should work with string type message") {
    auto compressed_msg = LZ4Compressor::compressData(msg);
  }

  SECTION("It should work with bytes type message") {
    vector<uint8_t> msg_bytes(msg.begin(), msg.end());

    auto compressed_msg = LZ4Compressor::compressData(msg_bytes);
  }
}

TEST_CASE("LZ4Compressor.decompressData") {
  string_view msg = "Hello world"sv;
  auto compressed_msg = LZ4Compressor::compressData(msg);

  SECTION("It should decompress string type message") {
    auto actual = LZ4Compressor::decompressData(compressed_msg);

    REQUIRE(actual == msg);
  }

  SECTION("It should decompress bytes type message") {
    vector<uint8_t> msg_bytes(msg.begin(), msg.end());
    auto compressed_msg_bytes = LZ4Compressor::compressData(msg_bytes);

    auto actual = LZ4Compressor::decompressData(compressed_msg_bytes);

    REQUIRE(actual == msg_bytes);
  }
}
