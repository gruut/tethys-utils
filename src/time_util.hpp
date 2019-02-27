#ifndef GRUUT_UTILS_TIME_HPP
#define GRUUT_UTILS_TIME_HPP

#include <chrono>
#include <string>

class TimeUtil {
public:
  static std::string now() {
    auto now = nowBigInt();

    return std::to_string(now);
  }

  static uint64_t nowBigInt() {
    auto now = static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count());

    return now;
  }

  static uint64_t nowMilSeconds() {
    auto milliseconds_since_epoch = static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count());

    return milliseconds_since_epoch;
  }
};

#endif
