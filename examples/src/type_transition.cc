#include <do_let_is/async.h>
#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <expected>
#include <optional>
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

struct summary {
  double scaled;
  std::string_view tag;
  constexpr bool operator==(const summary&) const = default;
};

constexpr auto convert_pipeline(std::optional<int> raw_count, std::optional<double> rate) {
  return DO(
    LET count IS(raw_count);
    LET unit_rate IS(rate);
    LET label IS(count > 0 ? std::optional<std::string_view>{"active"} : std::optional<std::string_view>{});
    RETURN(summary{.scaled = static_cast<double>(count) * unit_rate, .tag = label});
  );
}

static_assert(convert_pipeline(8, 1.25) == summary{.scaled = 10.0, .tag = "active"});
static_assert(convert_pipeline(0, 1.25) == std::nullopt);
static_assert(convert_pipeline(std::nullopt, 1.25) == std::nullopt);

int main() {
  doletis::executor ex;
  auto [t_count, p_count] = doletis::task<int>::pending();
  auto [t_rate, p_rate] = doletis::task<double>::pending();

  int cleanup = 0;
  auto job = DO(
    DEFER(cleanup += 1);
    LET n IS(t_count);
    LET r IS(t_rate);
    RETURN(summary{.scaled = static_cast<double>(n) * r, .tag = "done"});
  );

  p_count.resolve(16, &ex);
  p_rate.resolve(2.5, &ex);

  auto out = std::move(job).await(&ex);
  if (!out.has_value() || out->scaled != 40.0 || out->tag != "done" || cleanup != 1) {
    return 1;
  }
  return 0;
}
