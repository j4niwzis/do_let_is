#include "generator.h"

#include <algorithm>
#include <print>

// Allocation free generators

constexpr auto my_generator() {
  return GENERATOR((int i), (.i = 0),
    YIELD(42);
    WHILE(i != 10) (
      YIELD(i);
      ++i;
    )
    return doletis::generator_continuation<int, 24>();
  );
}
static_assert(std::ranges::equal(my_generator(),  // NOLINTNEXTLINE
                                 std::array{42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

int main() {
  auto gen = my_generator();
  gen.i = 3;
  std::println("{}", gen);  // prints [42, 3, 4, 5, 6, 7, 8, 9]
  /*
  for(auto i : gen) {
    std::println("{}", i);
  }
  */
}
