#include "variant.h"

constexpr auto variant_function(const std::variant<int, float>& a, const std::variant<int, double>& b) {
  return DO(
    LET a IS(a);
    LET b IS(b);
    return std::variant<double, int>{a + b};
  );
}
// NOLINTNEXTLINE
static_assert(std::get<double>(variant_function(3, 3.14)) == 6.1400000000000006);

// NOLINTNEXTLINE
static_assert(std::get<int>(variant_function(3, 3)) == 6);

int main() {}
