#ifndef GRUUT_UTILS_TYPE_CONVERTER_HPP
#define GRUUT_UTILS_TYPE_CONVERTER_HPP

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

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
};

#endif
