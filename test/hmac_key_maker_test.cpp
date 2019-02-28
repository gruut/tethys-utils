#include "../lib/catch.hpp"
#include "../src/hmac_key_maker.hpp"

using namespace std;

TEST_CASE("HmacKeyMaker") {
  SECTION("It should return the same symmetric key between users") {
    HmacKeyMaker bob_ecdh;
    HmacKeyMaker alice_ecdh;

    const auto &[alice_x_str, alice_y_str] = alice_ecdh.getPublicKey();
    const auto &[bob_x_str, bob_y_str] = bob_ecdh.getPublicKey();

    auto alice_symmetric_key = alice_ecdh.getSharedSecretKey(bob_x_str, bob_y_str);
    auto bob_symmetric_key = bob_ecdh.getSharedSecretKey(alice_x_str, alice_y_str);

    REQUIRE(alice_symmetric_key == bob_symmetric_key);
  }
}
