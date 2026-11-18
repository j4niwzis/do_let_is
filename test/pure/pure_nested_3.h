#ifndef PURE_NESTED_3_H
#define PURE_NESTED_3_H

#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <optional>

constexpr std::optional<int> pure_level3(int x) {
  return DO(
    LET v IS(std::optional<int>{x});
    RETURN(v + 10);
  );
}

#include <do_let_is/undef.h>

#endif
