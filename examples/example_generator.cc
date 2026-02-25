#include <print>

#include "generator.h"

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
  for(auto i : gen) {
    std::println("{}", i);
  }
}
