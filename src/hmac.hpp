#ifndef GRUUT_UTILS_HMAC_HPP
#define GRUUT_UTILS_HMAC_HPP

#include "sha256.hpp"
#include <botan-2/botan/mac.h>
#include <string>
#include <vector>

using namespace std;

class Hmac {
public:
  static vector<uint8_t> generateHMAC(const string &msg, vector<uint8_t> &key) {
    auto hmac = Botan::MessageAuthenticationCode::create_or_throw("HMAC(SHA-256)");

    hmac->set_key(Sha256::hash(key));
    hmac->update(msg);
    return hmac->final_stdvec();
  }

  static bool verifyHMAC(const string &msg, vector<uint8_t> &hmac, vector<uint8_t> &key) {
    auto hmac_verifier = Botan::MessageAuthenticationCode::create_or_throw("HMAC(SHA-256)");

    hmac_verifier->set_key(Sha256::hash(key));
    hmac_verifier->update(msg);
    return hmac_verifier->verify_mac(hmac);
  }
};

#endif //GRUUT_UTILS_HMAC_HPP
