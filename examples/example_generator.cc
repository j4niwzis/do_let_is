#include <print>

#include "generator.h"

// Allocation free generators

// clang-format off

auto my_generator() {
  return GENERATOR((int i), (.i = 0), 
    YIELD(42);
    WHILE(i != 10)(
      YIELD(i);
      ++i;
    )
    return rec<int, 16>();
  );
}
// clang-format on

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
