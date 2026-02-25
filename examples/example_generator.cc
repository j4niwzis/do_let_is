#include <print>

#include "generator.h"

// Allocation free generators

auto my_generator() {
  return GENERATOR((int i), (.i = 0),
    YIELD(42);
    WHILE(i != 10)(
      ++i;
      YIELD(i);
    )
    return rec<int, 16>{};
  );
}

int main() {
  auto gen = my_generator();
  std::println("{}", gen); // prints [42, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
  /*
  for(auto i : gen) {
    std::println("{}", i);
  }
  */
}
