#define DO_LET_IS_USE_BLOCKS
#include <do_let_is/all_short.h>

#include <cstdlib>
#include <expected>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

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

namespace blocks_cxx_ns {

template <typename T>
struct maybe {
  bool ok;
  T value;
};

template <typename T, typename F>
auto doletis_bind(const maybe<T>& m, F&& f) {
  using R = std::decay_t<std::invoke_result_t<F, const T&>>;
  return m.ok ? std::forward<F>(f)(m.value) : R{.ok = false, .value = {}};
}

template <typename T, typename V>
auto doletis_pure(::doletis::monad_tag<maybe<T>>, V&& v) {
  return maybe<std::decay_t<V>>{.ok = true, .value = std::forward<V>(v)};
}

template <typename T>
bool doletis_is_err(const maybe<T>& m) {
  return !m.ok;
}

}  // namespace blocks_cxx_ns

struct summary {
  double total;
  std::string_view tag;
  constexpr bool operator==(const summary&) const = default;
};

int main() {
  // 1) int -> double -> struct transition running on Clang -fblocks in C++23
  auto s = DO(
    LET n IS(std::optional<int>{12});
    LET k IS(std::optional<double>{3.5});
    RETURN(summary{.total = static_cast<double>(n) * k, .tag = "blocks"});
  );
  if (!s.has_value() || s->total != 42.0 || s->tag != "blocks") {
    abort();
  }

  // 2) DEFER + ERRDEFER + WHILE + IF with std::expected on Clang -fblocks in C++23
  int trace = 0;
  int* trace_p = &trace;
  auto exp = DO(
    DEFER(*trace_p = *trace_p * 10 + 1);
    ERRDEFER(*trace_p = *trace_p * 10 + 8);
    LET a IS(std::expected<int, int>{10});
    __block int i = 0;
    __block int acc = a;
    WHILE(i < 3)(
      DEFER(*trace_p = *trace_p * 10 + 3);
      acc += 5;
      ++i;
    )
    RETURN(acc);
  );
  if (!exp.has_value() || *exp != 25 || trace != 3331) {
    abort();
  }

  // 3) Custom ADL C++ monad wrapping std::string on Clang -fblocks
  int mtrace = 0;
  int* mtrace_p = &mtrace;
  auto m = DO(
    DEFER(*mtrace_p = *mtrace_p * 10 + 1);
    ERRDEFER(*mtrace_p = *mtrace_p * 10 + 9);
    LET pfx IS(blocks_cxx_ns::maybe<std::string>{.ok = true, .value = "v="});
    LET num IS(blocks_cxx_ns::maybe<int>{.ok = true, .value = 42});
    RETURN(pfx + std::to_string(num));
  );
  if (!m.ok || m.value != "v=42" || mtrace != 1) {
    abort();
  }
  return 0;
}
