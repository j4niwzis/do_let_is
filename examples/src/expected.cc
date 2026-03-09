#include "expected.h"

struct error {};

constexpr auto div(int a, int b) -> std::expected<int, error> {
  if (b == 0) {
    return std::unexpected(error{});
  }
  return a / b;
}

constexpr auto foo(std::expected<int, error> a, std::expected<int, error> b) {  //
  return DO(
    LET a IS(a);
    LET b IS(b);
    LET c IS(div(a, b));
    return std::expected<int, error>{c};
  );
}

static_assert(foo(5, 5) == 1);                                         // NOLINT
static_assert(foo(5, std::unexpected(error{})).has_value() == false);  // NOLINT
static_assert(foo(5, 0).has_value() == false);                         // NOLINT
int main() {}
