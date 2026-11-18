#ifndef PURE_NESTED_2_H
#define PURE_NESTED_2_H

#include <do_let_is/do_let_is.h>
#include <do_let_is/generator.h>
#include <do_let_is/short.h>

#include <optional>

constexpr std::optional<int> pure_level2_before(int x) {
  return DO(
    LET v IS(std::optional<int>{x});
    RETURN(v + 5);
  );
}

#include "pure_nested_3.h"

// Verify GENERATOR is still active in level 2 even though level 3 only included do_let_is.h + short.h!
constexpr auto pure_level2_gen() {
  return GENERATOR((int i), (.i = 0),
    WHILE(i < 3)(
      YIELD(i + 1);
      ++i;
    )
    return end<int, 16>();
  );
}

constexpr std::optional<int> pure_level2_after(int x) {
  return DO(
    LET a IS(pure_level2_before(x));
    LET b IS(pure_level3(a));
    RETURN(b);
  );
}

#include <do_let_is/undef.h>

#endif
