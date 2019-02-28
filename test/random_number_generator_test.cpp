#include <vector>

#include "../lib/catch.hpp"
#include "../src/random_number_generator.hpp"
#include "../src/type_converter.hpp"

using namespace std;

TEST_CASE("RandomNumGenerator.randomize") {
  SECTION("It should work") {
    auto random_num_bytes = RandomNumGenerator::randomize(1);

    const size_t expected = 1;

    REQUIRE(random_num_bytes.size() == expected);
  }

  SECTION("It should return random number in range") {
    auto actual = RandomNumGenerator::getRange(0, 10);

    bool is_in_range = 0 <= actual && actual <= 10;

    REQUIRE(is_in_range);
  }
}
