# do_let_is
do-notation DSL for C++

It is not so much needed in itself, but it is a demonstration of the capabilities of the preprocessor about the possibility of creating your own DSLs, a specific idea for parsing them. A full explanation of how it works will come later.

If a type supports

`bind(m, f)`

then it automatically works with this DSL:
```cpp
auto result = DO(
  LET x IS(mx);
  LET y IS(my);
  return make_value(x, y);
);
```

The syntax is the same regardless of the underlying monad.

This works for:
- std::optional
- std::vector (list monad semantics)
- std::variant (std::visit)
- custom types

but the DSL itself is completely type-agnostic. Everything is expressed in terms of bind.
### Design goals
- Unified do-notation syntax
- No C++20 coroutines
- No dynamic allocations introduced
- Works in constexpr contexts
- Linear code instead of nested lambdas




# Examples
 Without DSL:
```cpp
auto result = bind(mx, [&](auto x) {
  return bind(my, [&](auto y) {
    return make_value(x, y);
  });
});
```
With DSL:
```cpp
auto result = DO(
  LET x IS(mx);
  LET y IS(my);
  return make_value(x, y);
);
```
Loops and branches are also supported.

You can also easily create generators using this. [Example](https://github.com/j4niwzis/do_let_is/blob/main/examples/example_generator.cc) 


### Other examples

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
  return range
         | std::views::transform(std::forward<F>(f))
         | std::views::join
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

constexpr auto loop(const std::vector<std::optional<int>>& vec) {
  return DO(
    auto it = vec.begin();
    int result = 0;
    WHILE(it != vec.end())(
      LET value IS(*it);
      result += value;
      ++it;
    )
    return std::optional{result};
  );
}
static_assert(loop({1, 2, 3}) == 6);
static_assert(loop({1, std::nullopt, 3}) == std::nullopt);

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
      throw 42; // will never be executed
    );
    LET _ IS(std::optional{42});
    return std::optional<int>{c};
);

static_assert(c == 2);
```
