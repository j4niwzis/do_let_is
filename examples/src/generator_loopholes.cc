#include <do_let_is/generator_loopholes.h>
#include <do_let_is/short.h>

#include <algorithm>
#include <array>

constexpr auto my_generator() {  //
  return GENERATOR_LOOPHOLES((int i), (.i = 0),
    YIELD(42);
    WHILE(i != 10) (
      YIELD(i);
      ++i;
    )
    return end<int, 16>();
  );
}

static_assert(std::ranges::equal(my_generator(),  // NOLINTNEXTLINE
                                 std::array{42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

int main() {
  for (auto _ : my_generator()) {
  }
}
