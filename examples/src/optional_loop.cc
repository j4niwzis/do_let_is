#include <vector>

#include "optional.h"

#ifndef OPTIONAL_H
#endif

constexpr auto loop(const std::vector<std::optional<int>>& vec) {
  return DO(
    auto it = vec.begin();
    int result = 0;
    WHILE(it != vec.end())(
      LET value IS(*it);
      result += value;
      ++it;
    )
    return std::optional{result};
  );
}
static_assert(loop({1, 2, 3}) == 6);  // NOLINT
static_assert(loop({1, std::nullopt, 3}) == std::nullopt);

constexpr auto c = DO_GLOBAL(
    int c = 0;
    LET _ IS(std::optional{42});
    WHILE(c != 2)(
      IF(true) (
        struct {} i_can_write_code_there;
        LET v IS(std::optional{++c});
        struct { int x; } i_can_write_code_there_too{.x = v};
      )(
        BREAK;
      )
      IF_CONSTEXPR(false)(
        static_assert(false);
      )(
        CONTINUE;
      )
      throw 42; // will never be executed
    );
    LET _ IS(std::optional{42});
    return std::optional<int>{c};
);

static_assert(c == 2);

int main() {}
