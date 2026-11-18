#ifndef NESTED_CXX_BLOCKS_H
#define NESTED_CXX_BLOCKS_H

// Level 3.5: C++ header with Clang -fblocks enabled inside the nested chain!
#define DO_LET_IS_USE_BLOCKS
#include <do_let_is/all_short.h>

#include <optional>
#include <utility>

#ifndef DOLETIS_OPT_TRAITS_DEFINED
#define DOLETIS_OPT_TRAITS_DEFINED
template <typename T>
struct doletis::monad_traits<std::optional<T>> {
  template <typename Opt, typename F>
  static constexpr auto bind(Opt&& value, F&& f) {
    return std::forward<Opt>(value).and_then(std::forward<F>(f));
  }
};
#endif

// C++ std::optional + __block running on Clang blocks BEFORE nested pure-C++ header
inline std::optional<int> nested_cxx_blocks_before(int v) {
  return DO(
    LET x IS(std::optional<int>{v});
    __block int i = 0;
    __block int acc = x;
    WHILE(i < 2)(
      acc += 1;
      ++i;
    )
    RETURN(acc);
  );
}

// Include pure constexpr C++ header (gets standard C++ lambdas, NOT blocks!)
#include "nested_cxx_inner.h"

// After nested_cxx_inner.h's undef.h: C++-with-blocks mode is restored here!
inline std::optional<int> nested_cxx_blocks_after(int v, int* defer_cnt) {
  return DO(
    DEFER(*defer_cnt += 10);
    LET a IS(nested_cxx_blocks_before(v));
    LET b IS(nested_level4_cxx(0));
    __block int bonus = 0;
    WHILE(bonus < 1)(
      ++bonus;
    )
    RETURN(a + b + bonus);
  );
}

#include <do_let_is/undef.h>

#endif
