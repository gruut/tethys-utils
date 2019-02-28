#ifndef GRUUT_UTILS_HMAC_KEY_MAKER_HPP
#define GRUUT_UTILS_HMAC_KEY_MAKER_HPP

#include <iostream>

#include <botan-2/botan/auto_rng.h>
#include <botan-2/botan/bigint.h>
#include <botan-2/botan/curve_gfp.h>
#include <botan-2/botan/ec_group.h>
#include <botan-2/botan/ecdh.h>
#include <botan-2/botan/exceptn.h>
#include <botan-2/botan/hex.h>
#include <botan-2/botan/pubkey.h>

#include "./type_converter.hpp"

using namespace std;

class HmacKeyMaker {
public:
  HmacKeyMaker() :
          group_domain{"secp256r1"},
          kdf{"Raw"},
          curve{Botan::CurveGFp(group_domain.get_p(), group_domain.get_a(), group_domain.get_b())} {}

  pair<string, string> getPublicKey() {
    generateRandomSecretKey();

    Botan::ECDH_PrivateKey my_secret_key(rng, group_domain, secret_key);

    const string uncomp_point = Botan::hex_encode(my_secret_key.public_value());
    const size_t point_size = uncomp_point.size() - 2; // because of first 04 = means uncompressed point coordinator

    const size_t point_len = point_size / 2;

    const string my_public_key_x(uncomp_point.begin() + 2,
                                 uncomp_point.begin() + point_len + 2);
    const string my_public_key_y(uncomp_point.begin() + point_len + 2,
                                 uncomp_point.end());

    return make_pair(my_public_key_x, my_public_key_y);
  }

  vector<uint8_t>
  getSharedSecretKey(const string &your_public_key_x, const string &your_public_key_y, int key_len = 32) {
    Botan::secure_vector<uint8_t> symmetric_secret_key;

    try {
      Botan::ECDH_PrivateKey my_secret_key(rng, group_domain, secret_key);

      auto decoded_public_key_x = Botan::hex_decode(your_public_key_x);
      auto decoded_public_key_y = Botan::hex_decode(your_public_key_y);

      Botan::PointGFp your_public_key_point(
              curve, Botan::BigInt(decoded_public_key_x),
              Botan::BigInt(decoded_public_key_y));
      Botan::ECDH_PublicKey your_public_key(group_domain, your_public_key_point);

      Botan::PK_Key_Agreement new_ecdh(my_secret_key, rng, kdf);

      symmetric_secret_key = new_ecdh.derive_key(key_len, your_public_key.public_value()).bits_of();
    } catch (Botan::Exception &e) {
      cout << "ECDH: exception - " << e.what() << endl;
    }

    return vector<uint8_t>(symmetric_secret_key.begin(), symmetric_secret_key.end());
  }

private:
  void generateRandomSecretKey() {
    Botan::ECDH_PrivateKey new_rand_secret_key(rng, group_domain);
    secret_key = new_rand_secret_key.private_value();
  }

  Botan::AutoSeeded_RNG rng;
  Botan::EC_Group group_domain;
  Botan::CurveGFp curve;
  const string kdf;

  Botan::BigInt secret_key;
};

#endif
