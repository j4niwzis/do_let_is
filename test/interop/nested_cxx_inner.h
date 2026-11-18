#ifndef NESTED_CXX_INNER_H
#define NESTED_CXX_INNER_H

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

constexpr auto nested_level4_cxx(int v) {
  return DO(
    LET x IS(std::optional<int>{v});
    IF_CONSTEXPR(true)(
      RETURN(x + 4);
    )(
      RETURN(0);
    )
  );
}

static_assert(nested_level4_cxx(38) == 42);

#include <do_let_is/undef.h>

#endif
