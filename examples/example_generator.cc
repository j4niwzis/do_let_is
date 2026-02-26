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

auto my_generator2() {
  return GENERATOR_ES(
    int i = 0; // You can use state inside a generator as well.
    YIELD(42);
    WHILE(i != 10)(
      int i2 = 0;
      ++i;
      YIELD(i);
      YIELD(i);
      ++i2;
      YIELD(i2);
    )
    return rec<int, 40>{};
  );
}
// clang-format on

int main() {
  auto gen = my_generator();
  gen.i = 3;
  std::println("{}", gen);  // prints [42, 3, 4, 5, 6, 7, 8, 9]
  std::println("{}",
               my_generator2());  // prints [42, 1, 1, 1, 2, 2, 1, 3, 3, 1, 4, 4, 1, 5, 5, 1, 6, 6, 1, 7, 7, 1, 8, 8, 1, 9, 9, 1, 10, 10, 1]
  /*
  auto gen2 = my_generator2();
  for(auto i : gen2) {
    std::println("{}", i);
  }
  */
}
