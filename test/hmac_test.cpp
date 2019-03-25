#include "../lib/catch.hpp"
#include "../src/hmac.hpp"

using namespace std;

TEST_CASE("HMAC") {
  string msg = "Hello world";
  vector<uint8_t> real_key(0xFF, 32);
  vector<uint8_t> fake_key(0x00, 32);

  SECTION("It should generate and verify hmac correctly") {
    auto hmac = Hmac::generateHMAC(msg, real_key);

    REQUIRE(Hmac::verifyHMAC(msg, hmac, real_key) == true);
    REQUIRE(Hmac::verifyHMAC(msg, hmac, fake_key) == false);
  }
}
