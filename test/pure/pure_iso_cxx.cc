#include <expected>
#include <optional>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

#include <do_let_is/do_let_is.h>

template <typename T>
struct doletis::monad_traits<std::optional<T>> {
  template <typename Opt, typename F>
  static constexpr auto bind(Opt&& value, F&& f) {
    return std::forward<Opt>(value).and_then(std::forward<F>(f));
  }
};

template <typename T, typename Err>
struct doletis::monad_traits<std::expected<T, Err>> {
  template <typename Exp, typename F>
  static constexpr auto bind(Exp&& value, F&& f) {
    return std::forward<Exp>(value).and_then(std::forward<F>(f));
  }
};

// 1. Test Boost.PP-style #include counter in pure ISO C++23 (-std=c++23 -pedantic-errors)
#include <do_let_is/counter.h>
static_assert(DO_LET_IS_COUNTER == 0);
#include DO_LET_IS_UPDATE_COUNTER()
static_assert(DO_LET_IS_COUNTER == 1);
#include DO_LET_IS_UPDATE_COUNTER()
#include DO_LET_IS_UPDATE_COUNTER()
#include DO_LET_IS_UPDATE_COUNTER()
#include DO_LET_IS_UPDATE_COUNTER()
#include DO_LET_IS_UPDATE_COUNTER()
#include DO_LET_IS_UPDATE_COUNTER()
#include DO_LET_IS_UPDATE_COUNTER()
static_assert(DO_LET_IS_COUNTER == 8);
#include <do_let_is/update_counter.h>
#include <do_let_is/update_counter.h>
#include <do_let_is/update_counter.h>
#include <do_let_is/update_counter.h>
static_assert(DO_LET_IS_COUNTER == 12);

// 2. Include 3-level nested pure C++ header chain before short.h is active here
#include "pure_nested_1.h"

#ifdef DO
#error "DO must be undefined after pure_nested_1.h popped back to empty stack"
#endif
#ifdef GENERATOR
#error "GENERATOR must be undefined after pure_nested_1.h popped back to empty stack"
#endif
#ifdef GENERATOR_LOOPHOLES
#error "GENERATOR_LOOPHOLES must be undefined after pure_nested_1.h popped back to empty stack"
#endif

// Long-form DO_LET_IS_DO still works when short macros are undefined
static_assert(DO_LET_IS_DO_GLOBAL(DO_LET_IS_LET x DO_LET_IS_IS(std::optional<int>{40}); DO_LET_IS_RETURN(x + 2);) == 42);

// 3. Activate short macros in .cc file
#include <do_let_is/async.h>
#include <do_let_is/do_let_is.h>
#include <do_let_is/generator.h>
#include <do_let_is/short.h>

static_assert(pure_level1_after(25) == 42);

struct out_box {
  double total;
  std::string_view tag;
  constexpr bool operator==(const out_box&) const = default;
};

constexpr auto check_pipeline(const std::vector<std::optional<int>>& xs) {
  return DO(
    auto it = xs.begin();
    int sum = 0;
    WHILE(it != xs.end())(
      LET v IS(*it);
      sum += v;
      ++it;
    )
    RETURN(sum);
  );
}

static_assert(check_pipeline({10, 12, 20}) == 42);
static_assert(check_pipeline({10, std::nullopt, 20}) == std::nullopt);

constexpr auto check_expected(bool fail) {
  int cleanup = 0;
  auto res = DO(
    DEFER(cleanup += 1);
    ERRDEFER(cleanup += 10);
    IF(fail)(
      RETURN(std::unexpected{-3});
    )(
      ;
    )
    LET n IS(std::expected<int, int>{12});
    LET k IS(std::expected<double, int>{3.5});
    LET tag IS(std::expected<std::string_view, int>{"pure"});
    RETURN(out_box{.total = static_cast<double>(n) * k, .tag = tag});
  );
  return std::pair{res, cleanup};
}

static_assert(check_expected(false).first == out_box{.total = 42.0, .tag = "pure"});
static_assert(check_expected(false).second == 1);
static_assert(check_expected(true).first.error() == -3);
static_assert(check_expected(true).second == 11);

int main() {
  auto v1 = pure_level2_gen() | std::ranges::to<std::vector>();
  if (v1 != (std::vector<int>{1, 2, 3})) {
    return 1;
  }
  auto v2 = pure_level1_gen() | std::ranges::to<std::vector>();
  if (v2 != (std::vector<int>{0, 10, 20})) {
    return 1;
  }
  return 0;
}
