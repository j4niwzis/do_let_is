#ifndef NESTED_CXX_OUTER_H
#define NESTED_CXX_OUTER_H

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

// Level 1 C++ DO before including C header chain
constexpr auto nested_level1_cxx_before(int v) {
  return DO(
    LET x IS(std::optional<int>{v});
    IF_CONSTEXPR(true)(
      RETURN(x + 1);
    )(
      RETURN(0);
    )
  );
}

static_assert(nested_level1_cxx_before(41) == 42);

// Include C header -> C header -> C++ header!
#include "nested_c_1.h"

// Level 1 C++ DO + GENERATOR_LOOPHOLES automatically restored after C header chain!
inline std::optional<int> nested_level1_cxx_after(int start, int* defer_cnt) {
  return DO(
    LET x IS(nested_level1_cxx_before(start));
    nested_c1_opt mid = nested_level2_c_after(x, defer_cnt);
    LET y IS(mid.has_value ? std::optional<int>{mid.value} : std::optional<int>{});
    RETURN(y);
  );
}

constexpr auto nested_level1_loophole_gen() {
  return GENERATOR_LOOPHOLES((int i), (.i = 0),
    WHILE(i < 3)(
      YIELD(i + 1);
      ++i;
    )
    return end<int, 16>();
  );
}

#include <do_let_is/undef.h>

#endif
