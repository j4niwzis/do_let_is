#include "optional.h"

constexpr auto optional_function(const std::optional<int>& a, const std::optional<int>& b) {
  return DO(
      LET a IS(a);
      LET b IS(b);
      return std::optional{a + b};
  );
}

static_assert(optional_function(2, 2) == 4);
static_assert(optional_function({}, 2) == std::nullopt);
static_assert(optional_function(2, {}) == std::nullopt);
static_assert(optional_function({}, {}) == std::nullopt);

int main() {}
