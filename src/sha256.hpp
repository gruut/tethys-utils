#ifndef TETHYS_UTILS_SHA_256_HPP
#define TETHYS_UTILS_SHA_256_HPP

#include <botan-2/botan/base64.h>
#include <botan-2/botan/hash.h>
#include <sstream>
#include <string>
#include <vector>

#include "type_converter.hpp"

using namespace std;

class Sha256 {
  using hash_t = vector<uint8_t>;

public:
  inline static bool isMatch(string_view target_message, const hash_t &hashed_message) {
    auto hashed_target_message = hash(target_message);
    bool result = hashed_target_message == hashed_message;

    return result;
  }

  inline static hash_t hash(string_view message) {
    auto msg_bytes = TypeConverter::stringToBytes(message);
    return hash(msg_bytes);
  }

  template <typename T>
  static hash_t hash(T &&msg_bytes) {
    unique_ptr<Botan::HashFunction> hash_function(Botan::HashFunction::create("SHA-256"));
    hash_function->update(msg_bytes);

    return hash_function->final_stdvec();
  }

  template <size_t S>
  static hash_t hash(const array<uint8_t, S> &msg_bytes_array) {
    unique_ptr<Botan::HashFunction> hash_function(Botan::HashFunction::create("SHA-256"));

    auto msg_bytes = vector<uint8_t>(msg_bytes_array.begin(), msg_bytes_array.end());

    hash_function->update(msg_bytes);
    return hash_function->final_stdvec();
  }
};

#endif
