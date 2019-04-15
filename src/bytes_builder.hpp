#ifndef GRUUT_UTILS_BYTES_BUILDER_HPP
#define GRUUT_UTILS_BYTES_BUILDER_HPP

#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

#include <iostream>

#include "type_converter.hpp"

using namespace std;

class BytesBuilder {
public:
  BytesBuilder() = default;
  ~BytesBuilder() = default;

  template <typename T>
  void append(T &bytes_val) {
    bytes.insert(bytes.end(), begin(bytes_val), end(bytes_val));
  }

  void appendHex(string_view hex_str) {
    int len = hex_str.length() / 2;
    vector<uint8_t> temp_vec;
    for (int i = 0; i < len; i++) {
      auto one_byte = (uint8_t)stoi("0x" + string(hex_str.substr(i * 2, 2)), nullptr, 16);
      temp_vec.push_back(one_byte);
    }
    append(temp_vec);
  }

  void appendDec(string_view dec_str) {
    uint64_t dec = stoull(string(dec_str));
    for (int i = sizeof(dec) - 1; i >= 0; i--) {
      bytes.push_back((dec >> (8 * i)) & 0xFF);
    }
  }

  template <unsigned int S>
  void appendBase(string_view baseXX_str) {
    auto decoded_data = TypeConverter::decodeBase<S>(baseXX_str);
    append(decoded_data);
  }

  vector<uint8_t> getBytes() {
    return bytes;
  }

  string getString() {
    return string(bytes.begin(), bytes.end());
  }

private:
  vector<uint8_t> bytes;
};

#endif // GRUUT_UTILS_BYTES_BUILDER_HPP
