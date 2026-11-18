#ifndef PURE_NESTED_1_H
#define PURE_NESTED_1_H

#include <do_let_is/async.h>
#include <do_let_is/do_let_is.h>
#include <do_let_is/generator.h>
#include <do_let_is/short.h>

#include <optional>

constexpr std::optional<int> pure_level1_before(int x) {
  return DO(
    LET v IS(std::optional<int>{x});
    RETURN(v + 2);
  );
}

#include "pure_nested_2.h"

// Verify GENERATOR and DO are both restored in level 1 after level 2 & level 3!
constexpr auto pure_level1_gen() {
  return GENERATOR((int i), (.i = 0),
    WHILE(i < 3)(
      YIELD(i * 10);
      ++i;
    )
    return end<int, 16>();
  );
}

constexpr std::optional<int> pure_level1_after(int x) {
  return DO(
    LET a IS(pure_level1_before(x));
    LET b IS(pure_level2_after(a));
    RETURN(b);
  );
}

#include <do_let_is/undef.h>

#endif
