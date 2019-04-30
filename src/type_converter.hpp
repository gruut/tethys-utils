#ifndef GRUUT_UTILS_TYPE_CONVERTER_HPP
#define GRUUT_UTILS_TYPE_CONVERTER_HPP

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

#include <botan-2/botan/base58.h>
#include <botan-2/botan/base64.h>
#include <botan-2/botan/exceptn.h>
#include <botan-2/botan/secmem.h>
#include <iostream>

using namespace std;

template<unsigned int>
struct Encoder;

template<>
struct Encoder<58> {
  static auto encode(const vector<uint8_t> &&base_str) {
    auto encoded = Botan::base58_encode(base_str);
    return string(encoded.begin(), encoded.end());
  }
};

template<>
struct Encoder<64> {
  static auto encode(const vector<uint8_t> &&base_str) {
    auto encoded = Botan::base64_encode(base_str);
    return string(encoded.begin(), encoded.end());
  }
};

template<unsigned int>
struct Decoder;

template<>
struct Decoder<58> {
  static auto decode(const string_view &base_str) {
    auto decoded = Botan::base58_decode(base_str.data());
    return string(decoded.begin(), decoded.end());
  }
};

template<>
struct Decoder<64> {
  static auto decode(const string_view &base_str) {
    auto decoded = Botan::base64_decode(base_str.data());
    return string(decoded.begin(), decoded.end());
  }
};

class TypeConverter {
public:
  template<class T>
  inline static vector<uint8_t> integerToBytes(T &&input) {
    std::vector<uint8_t> v;
    auto input_size = sizeof(input);

    v.reserve(input_size);

    for (auto i = 0; i < input_size; ++i) {
      v.push_back(static_cast<uint8_t>(input & 0xFF));
      input >>= 8;
    }

    reverse(v.begin(), v.end());
    return v;
  }

  template<size_t ArraySize, typename T = std::vector<uint8_t>>
  inline static std::array<uint8_t, ArraySize> bytesToArray(T &&bytes) {
    using Array = std::array<uint8_t, ArraySize>;

    size_t copyable_size = (bytes.size() >= ArraySize) ? ArraySize : bytes.size();

    Array arr;
    std::copy(bytes.begin(), bytes.begin() + copyable_size, arr.begin());

    return arr;
  }

  template<unsigned int S, size_t ArraySize>
  inline static std::array<uint8_t, ArraySize> arrayFromBase(const string_view &basexx_str) {
    using Array = std::array<uint8_t, ArraySize>;

    string_view decoded_string = decodeBase<S>(basexx_str);

    size_t len;
    if (decoded_string.size() > S)
      len = S;
    else
      len = decoded_string.size();

    Array arr;
    std::copy(decoded_string.begin(), decoded_string.begin() + len, arr.begin());

    return arr;
  }

  template<unsigned int S>
  inline static string decodeBase(const string_view &base_str) {
    using decoder = Decoder<S>;

    try {
      return decoder::decode(base_str);
    } catch (Botan::Exception &e) {
      std::cout << e.what() << std::endl;
    }

    return string();
  }

  template<typename T = std::vector<uint8_t>>
  inline static std::string bytesToString(T &&input) {
    return std::string(input.begin(), input.end());
  }

  template<size_t S, class Array = array<uint8_t, S>>
  inline static std::string arrayToString(Array &&arr) {
    std::string str(arr.begin(), arr.end());
    return str;
  }

  inline static std::vector<uint8_t> stringToBytes(const string_view &input) {
    return std::vector<uint8_t>(input.begin(), input.end());
  }

  template<unsigned int S, typename T>
  inline static string encodeBase(T &&t) {
    using encoder = Encoder<S>;

    try {
      return encoder::encode(vector<uint8_t>(begin(t), end(t)));
    } catch (Botan::Exception &e) {
      std::cout << e.what() << std::endl;
    }

    return std::string("");
  }

  template<class Container>
  static inline std::string toString(const Container &&bytes) {
    return std::string(bytes.cbegin(), bytes.cend());
  }
};

#endif
