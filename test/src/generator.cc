#include "do_let_is/generator.h"

#include <gtest/gtest.h>

#include <ranges>
#include <vector>

#include "do_let_is/short.h"

namespace {

constexpr auto my_generator() {  //
  return GENERATOR((int i), (.i = 0),
    YIELD(42);
    WHILE(i != 10) (
      YIELD(i);
      ++i;
    )
    return end<int, 16>();
  );
}

}  // namespace

TEST(Generator, Base) {  //
  EXPECT_EQ(my_generator() | std::ranges::to<std::vector>(), (std::vector{42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
}
