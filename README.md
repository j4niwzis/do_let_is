# do_let_is
do-notation DSL for C++

# Examples

```cpp
#include <optional>
#include <ranges>
#include <vector>
#include <variant>

template <typename T, typename F>
constexpr auto bind(const std::optional<T>& value, F&& f) -> decltype(value.and_then(std::forward<F>(f))) {
  return value.and_then(std::forward<F>(f));
}

constexpr auto optional_function(const std::optional<int>& a, const std::optional<int>& b) {
  return DO(
      LET a IS(a);
      LET b IS(b);
      return std::optional{a + b};
  );
}

template <typename T, typename F>
constexpr auto bind(const std::vector<T>& range, F&& f) -> std::invoke_result_t<F, T> {
  return range                                        //
         | std::views::transform(std::forward<F>(f))  //
         | std::views::join                           //
         | std::ranges::to<std::vector>();
}

constexpr auto list_function(const std::vector<int>& a, const std::vector<int>& b) {
  return DO(
      LET a IS(a);
      LET b IS(b);
      return std::vector{a + b, a + b};
  );
}

static_assert(list_function({1, 2, 3}, {1, 2, 3}) == std::vector{2, 2, 3, 3, 4, 4, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6});

template <typename... Ts, typename F>
constexpr auto bind(const std::variant<Ts...>& var, F&& f) {
  return std::visit(std::forward<F>(f), var);
}

constexpr auto variant_function(const std::variant<int, float>& a, const std::variant<int, double>& b) {
  return DO(
    LET a IS(a);
    LET b IS(b);
    return std::variant<double, int>{a + b};
  );
}

static_assert(std::get<double>(variant_function(3, 3.14)) == 6.1400000000000006);
static_assert(std::get<int>(variant_function(3, 3)) == 6);

constexpr auto c = DO_GLOBAL(
    int c = 0;
    LET _ IS(std::optional{42});
    WHILE(c != 2)(
      IF(true) (
        struct {} i_can_write_code_there;
        LET v IS(std::optional{++c});
        struct { int x; } i_can_write_code_there_too{.x = v};
      )(
        BREAK;
      )
      IF_CONSTEXPR(false)(
        static_assert(false);
      )(
        CONTINUE;
      )
    );
    LET _ IS(std::optional{42});
    return std::optional<int>{c};
);

static_assert(c == 2);
```
