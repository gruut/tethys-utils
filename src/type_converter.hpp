#ifndef GRUUT_UTILS_TYPE_CONVERTER_HPP
#define GRUUT_UTILS_TYPE_CONVERTER_HPP

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

#include <botan-2/botan/base64.h>
#include <botan-2/botan/exceptn.h>
#include <botan-2/botan/secmem.h>

using namespace std;

class TypeConverter {
public:
  template<class T>
  inline static vector<uint8_t> integerToBytes(T input) {
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

    size_t copyableSize = (bytes.size() >= ArraySize) ? ArraySize : bytes.size();

    Array arr;
    std::copy(bytes.begin(), bytes.begin() + copyableSize, arr.begin());

    return arr;
  }

  template <size_t S>
  inline static std::array<uint8_t, S> base64ToArray(string_view b64_str) {
    using Array = std::array<uint8_t, S>;

    string decoded_string = decodeBase64(b64_str);

    size_t len = (decoded_string.size() >= S) ? S : decoded_string.size();

    Array arr;
    std::copy(decoded_string.begin(), decoded_string.begin() + len, arr.begin());

    return arr;
  }

  inline static string decodeBase64(string_view b64_str) {
    try {
      auto decoded = Botan::base64_decode(static_cast<string>(b64_str));
      return string(decoded.begin(), decoded.end());
    } catch (Botan::Exception &e) {
      std::cout << e.what() << std::endl;
    }

    return string();
  }
};

#endif
