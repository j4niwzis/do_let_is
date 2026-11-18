#include <gtest/gtest.h>

#include <expected>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "do_let_is/async.h"
#include "do_let_is/do_let_is.h"
#include "do_let_is/short.h"

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

namespace custom_ns {

// Custom monad in its own namespace: resolved purely via ADL (no doletis::monad_traits needed)
template <typename T>
struct maybe {
  bool ok;
  T value;
  constexpr bool operator==(const maybe&) const = default;
};

template <typename T, typename F>
constexpr auto doletis_bind(const maybe<T>& m, F&& f) {
  using R = std::decay_t<std::invoke_result_t<F, const T&>>;
  return m.ok ? std::forward<F>(f)(m.value) : R{.ok = false, .value = {}};
}

template <typename T, typename V>
constexpr auto doletis_pure(::doletis::monad_tag<maybe<T>>, V&& v) {
  if constexpr (std::is_same_v<std::decay_t<V>, maybe<T>>) {
    return std::forward<V>(v);
  } else {
    return maybe<std::decay_t<V>>{.ok = true, .value = std::forward<V>(v)};
  }
}

template <typename T>
constexpr bool doletis_is_err(const maybe<T>& m) {
  return !m.ok;
}

}  // namespace custom_ns

namespace {

constexpr int run_constexpr_defer(int mode) {
  int log = 0;
  auto res = DO(
    DEFER(log = log * 10 + 1);
    ERRDEFER(log = log * 10 + 8);
    LET a IS(mode == 1 ? std::expected<int, int>{std::unexpect, -1} : std::expected<int, int>{10});
    DEFER(log = log * 10 + 2);
    int i = 0;
    int sum = a;
    WHILE(i < 3)(
      DEFER(log = log * 10 + 3);
      ERRDEFER(log = log * 10 + 9);
      IF(mode == 2 && i == 1)(
        RETURN(std::unexpected{-2});
      )(
        IF(i == 2)(
          BREAK;
        )(
          sum += 5;
          ++i;
        )
      )
    )
    RETURN(sum);
  );
  if (mode == 0 && (!res.has_value() || *res != 20)) {
    return -999;
  }
  return log;
}

static_assert(run_constexpr_defer(0) == 33321);
static_assert(run_constexpr_defer(1) == 81);
static_assert(run_constexpr_defer(2) == 393281);

TEST(DeferAndAsync, ConstexprDefer) {
  EXPECT_EQ(run_constexpr_defer(0), 33321);
  EXPECT_EQ(run_constexpr_defer(1), 81);
  EXPECT_EQ(run_constexpr_defer(2), 393281);
}

TEST(DeferAndAsync, AsyncPipelineAndTrampoline) {
  doletis::executor ex;

  int i = 0;
  int sum = 0;
  auto heavy = DO(
    WHILE(i < 50000)(
      LET x IS(doletis::ready(1));
      sum += x;
      ++i;
    )
    RETURN(sum);
  );
  EXPECT_EQ(std::move(heavy).await(&ex), std::optional<int>{50000});

  auto [t1, p1] = doletis::task<int>::pending();
  auto [t2, p2] = doletis::task<int>::pending();
  int log = 0;
  auto job = DO(
    DEFER(log = log * 10 + 1);
    ERRDEFER(log = log * 10 + 9);
    LET a IS(t1);
    DEFER(log = log * 10 + 2);
    LET b IS(t2);
    RETURN(a + b);
  );
  p2.resolve(30, &ex);
  p1.resolve(12, &ex);
  EXPECT_EQ(std::move(job).await(&ex), std::optional<int>{42});
  EXPECT_EQ(log, 21);
}

TEST(DeferAndAsync, AsyncCancelRunsErrdeferAndDefer) {
  doletis::executor ex;
  auto [t1, p1] = doletis::task<int>::pending();
  auto [t2, p2] = doletis::task<int>::pending();
  int log = 0;
  auto job = DO(
    DEFER(log = log * 10 + 1);
    ERRDEFER(log = log * 10 + 8);
    LET a IS(t1);
    (void)a;
    DEFER(log = log * 10 + 2);
    ERRDEFER(log = log * 10 + 9);
    LET b IS(t2);
    RETURN(b);
  );
  p1.resolve(10, &ex);
  ex.run();
  EXPECT_EQ(log, 0);
  job.cancel();
  EXPECT_EQ(log, 9281);
}

struct item_summary {
  double total;
  std::string_view label;
  constexpr bool operator==(const item_summary&) const = default;
};

constexpr auto constexpr_type_transition(bool fail) {
  return DO(
    LET count IS(fail ? std::expected<int, int>{std::unexpect, -7} : std::expected<int, int>{8});
    LET ratio IS(std::expected<double, int>{5.25});
    LET tag IS(std::expected<std::string_view, int>{"ok"});
    IF_CONSTEXPR(std::is_same_v<int, int>)(
      RETURN(item_summary{.total = static_cast<double>(count) * ratio, .label = tag});
    )(
      RETURN(item_summary{.total = 0.0, .label = "bad"});
    )
  );
}

static_assert(constexpr_type_transition(false) == item_summary{.total = 42.0, .label = "ok"});
static_assert(constexpr_type_transition(true).error() == -7);

constexpr auto adl_custom_monad_test(bool fail, int* trace) {
  return DO(
    DEFER(*trace = *trace * 10 + 1);
    ERRDEFER(*trace = *trace * 10 + 9);
    LET n IS(custom_ns::maybe<int>{.ok = !fail, .value = 20});
    LET k IS(custom_ns::maybe<double>{.ok = true, .value = 2.1});
    RETURN(static_cast<double>(n) * k);
  );
}

static_assert([] {
  int t = 0;
  auto r = adl_custom_monad_test(false, &t);
  return r.ok && r.value == 42.0 && t == 1;
}());

static_assert([] {
  int t = 0;
  auto r = adl_custom_monad_test(true, &t);
  return !r.ok && t == 91;
}());

TEST(DeferAndAsync, TypeTransitionSyncAndAsync) {
  EXPECT_EQ(*constexpr_type_transition(false), (item_summary{.total = 42.0, .label = "ok"}));

  auto str_monad = DO(
    LET prefix IS(custom_ns::maybe<std::string>{.ok = true, .value = "ans="});
    LET num IS(custom_ns::maybe<int>{.ok = true, .value = 42});
    RETURN(prefix + std::to_string(num));
  );
  EXPECT_TRUE(str_monad.ok);
  EXPECT_EQ(str_monad.value, "ans=42");

  doletis::executor ex;
  auto [t_int, p_int] = doletis::task<int>::pending();
  auto [t_dbl, p_dbl] = doletis::task<double>::pending();
  int cleanup = 0;
  auto job = DO(
    DEFER(cleanup += 1);
    LET n IS(t_int);
    LET factor IS(t_dbl);
    RETURN(std::to_string(static_cast<int>(n * factor)));
  );
  p_int.resolve(14, &ex);
  p_dbl.resolve(3.0, &ex);
  EXPECT_EQ(std::move(job).await(&ex), std::optional<std::string>{"42"});
  EXPECT_EQ(cleanup, 1);
}

}  // namespace
