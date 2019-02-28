#include <vector>

#include "../lib/catch.hpp"
#include "../src/sha256.hpp"

using namespace std;

TEST_CASE("Sha256") {
  string_view msg = "Hello world"sv;

  auto hashed_message = Sha256::hash(msg);
  SECTION("Message should be matched with a hashed message") {
    auto actual = Sha256::hash(msg);
    auto expected = hashed_message;

    REQUIRE(actual == expected);
    REQUIRE(Sha256::isMatch(msg, actual) == true);
  }

  SECTION("Bytes should be hashed correctly") {
    vector<uint8_t> msg_bytes(msg.begin(), msg.end());
    auto actual = Sha256::hash(msg_bytes);

    REQUIRE(Sha256::isMatch(msg, actual));
  }
}
