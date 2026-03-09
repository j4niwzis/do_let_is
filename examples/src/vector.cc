#include "vector.h"

constexpr auto list_function(const std::vector<int>& a, const std::vector<int>& b) {
  return DO(
      LET a IS(a);
      LET b IS(b);
      return std::vector{a + b, a + b};
  );
}
// NOLINTNEXTLINE
static_assert(list_function({1, 2, 3}, {1, 2, 3}) == std::vector{2, 2, 3, 3, 4, 4, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6});

int main() {}
