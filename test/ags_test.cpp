#include "../lib/catch.hpp"
#include "../src/ags.hpp"
#include "../src/random_number_generator.hpp"
#include "../src/sha256.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MIN_INT = -2147483648;
const int MAX_INT = 2147483647;
const int TEST_SET_SIZE = 8000;

Botan::EC_Group group_domain{"secp256k1"};
Botan::AutoSeeded_RNG rng;

struct Key {
  Botan::BigInt sk;
  Botan::PointGFp pk;
  string encoded_pk;
};

string randomMsgGen() {
  vector<uint8_t> random_vec;
  auto num = RandomNumGenerator::getRange(MIN_INT, MAX_INT);
  auto str_num = to_string(num);
  auto hashed_data = Sha256::hash(str_num);
  return string(hashed_data.begin(), hashed_data.end());
}

Key keyGen() {
  Key key;
  key.sk = group_domain.random_scalar(rng);
  vector<Botan::BigInt> temp_vec;
  key.pk = group_domain.blinded_base_point_multiply(key.sk, rng, temp_vec);
  auto vec_encoded_pk = key.pk.encode(Botan::PointGFp::UNCOMPRESSED);
  key.encoded_pk = string(vec_encoded_pk.begin(), vec_encoded_pk.end());
  return key;
}

TEST_CASE("AGS") {
  vector<string> test_msg;
  vector<Key> test_key;
  for (int i = 0; i < TEST_SET_SIZE; i++) {
    test_msg.emplace_back(randomMsgGen());
    test_key.emplace_back(keyGen());
  }

  SECTION("single sign & verify") {
    AGS ags;
    auto sig = ags.sign(test_key[0].sk, test_msg[0]);
    auto actual = ags.verify(test_key[0].encoded_pk, test_msg[0], sig.value());

    bool expected = true;
    REQUIRE(actual == expected);
  }
  SECTION("aggregate & aggregateVerify") {
    AGS ags;
    vector<AggregateSet> test_agg_set;
    for (int i = 0; i < TEST_SET_SIZE; i++) {
      AggregateSet agg_set;
      agg_set.encoded_pk = test_key[i].encoded_pk;
      agg_set.msg = test_msg[i];
      agg_set.sig = ags.sign(test_key[i].sk, test_msg[i]).value();
      test_agg_set.emplace_back(agg_set);
    }
    Botan::BigInt res_z;
    auto agg_sig_set = ags.aggregate(test_agg_set, res_z);
    auto actual = ags.aggregateVerify(agg_sig_set.value(), res_z);

    bool expected = true;
    REQUIRE(actual == expected);
  }
}