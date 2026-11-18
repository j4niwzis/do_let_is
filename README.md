# do_let_is
do-notation DSL for C++23 and C23 (Clang `-fblocks`)

It is not so much needed in itself, but it is a demonstration of the capabilities of the preprocessor about the possibility of creating your own DSLs, a specific idea for parsing them. A full explanation of how it works will come later.

If a type supports `doletis_bind(m, f)` via ADL (or `doletis::monad_traits<M>::bind` for foreign types), it works with this DSL:
```cpp
auto result = DO(
  LET x IS(mx);
  LET y IS(my);
  RETURN(x + y);
);
```

The syntax is the same regardless of the underlying monad:
- `std::optional`, `std::expected`, `std::vector`, `std::variant` (via `doletis::monad_traits<M>`)
- `doletis::task<T>` (stackless CPS async) and `GENERATOR` / `GENERATOR_LOOPHOLES`
- custom types in C++23 (via ADL) and C23 (via `__attribute__((overloadable))`)

### Design goals
- Unified do-notation syntax across C++23 and C23 (`-fblocks`)
- No C++20 coroutines
- No dynamic allocations introduced in core `DO` or `GENERATOR`
- Works in `constexpr` contexts
- `DEFER`, `ERRDEFER`, and `RETURN(v)` (with monad deduction via `pure`) across scopes, loops, short-circuits, and async cancellation
- Safe C-in-C++ header interop (`<do_let_is/c/all_short.h>` + `<do_let_is/undef.h>`) and opt-in Clang `-fblocks` backend for C++ (`#define DO_LET_IS_USE_BLOCKS`)


# Defining Custom Monads

### C++: ADL (recommended for your own types)

Define `doletis_bind`, `doletis_pure`, and optional `doletis_is_err` (or `doletis_bind_defer` for async types) in the type's own namespace (prefixed to avoid ADL ambiguity with `std::bind`):

```cpp
#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

namespace app {

template <typename T>
struct maybe {
  bool ok;
  T value;
};

template <typename T, typename F>
constexpr auto doletis_bind(const maybe<T>& m, F&& f) {
  using R = std::decay_t<std::invoke_result_t<F, const T&>>;
  return m.ok ? std::forward<F>(f)(m.value) : R{.ok = false, .value = {}};
}

template <typename T, typename V>
constexpr auto doletis_pure(doletis::monad_tag<maybe<T>>, V&& v) {
  return maybe<std::decay_t<V>>{.ok = true, .value = std::forward<V>(v)};
}

template <typename T>
constexpr bool doletis_is_err(const maybe<T>& m) {
  return !m.ok;
}

}  // namespace app
```

### C++: `doletis::monad_traits<M>` (for foreign / `std` types)

When you cannot add functions to a foreign namespace such as `std`, specialize `doletis::monad_traits<M>`:

```cpp
template <typename T>
struct doletis::monad_traits<std::optional<T>> {
  template <typename Opt, typename F>
  static constexpr auto bind(Opt&& value, F&& f) {
    return std::forward<Opt>(value).and_then(std::forward<F>(f));
  }
  // optional: static constexpr auto pure(V&& v);
  // optional: static constexpr bool is_err(const std::optional<T>& v);
};
```

### C23 (`clang -std=c23 -fblocks -lBlocksRuntime`)

Define `doletis_elem` (declaration only, for element type deduction), `doletis_bind`, `doletis_pure`, and optional `doletis_is_err_impl` with `DO_LET_IS_OVERLOAD`:

```c
#include <do_let_is/c/do_let_is.h>
#include <do_let_is/short.h>

typedef struct {
  bool ok;
  int value;
} maybe_int;

DO_LET_IS_OVERLOAD int doletis_elem(maybe_int);

DO_LET_IS_OVERLOAD static inline maybe_int doletis_pure(maybe_int _tag, int v) {
  (void)_tag;
  return (maybe_int){.ok = true, .value = v};
}

DO_LET_IS_OVERLOAD static inline maybe_int doletis_pure(maybe_int _tag, maybe_int m) {
  (void)_tag;
  return m;
}

DO_LET_IS_OVERLOAD static inline bool doletis_is_err_impl(int _tag, maybe_int m) {
  (void)_tag;
  return !m.ok;
}

DO_LET_IS_OVERLOAD static inline maybe_int doletis_bind(maybe_int m, maybe_int (^f)(int)) {
  return m.ok ? f(m.value) : (maybe_int){.ok = false};
}
```

Or use the built-in generators in `<do_let_is/c/monads.h>`, `<do_let_is/c/generator.h>`, and `<do_let_is/c/async.h>`:
`DOLETIS_DEFINE_OPTIONAL`, `DOLETIS_DEFINE_EXPECTED`, `DOLETIS_DEFINE_VEC`, `DOLETIS_DEFINE_GENERATOR`, `DOLETIS_DEFINE_TASK` (plus `*_CONV` for cross-type transitions).

### C / C++ Interop, Pure Preprocessor Header Stack & Counter, and Clang `-fblocks` Mode in C++

- **Pure C++23 core with zero pragmas or compiler extensions:** `<do_let_is/do_let_is.h>`, `<do_let_is/generator.h>`, `<do_let_is/generator_loopholes.h>`, `<do_let_is/async.h>`, `<do_let_is/short.h>`, `<do_let_is/undef.h>`, and `<do_let_is/counter.h>` use only standard ISO C++23 and standard `#if` / `#include` preprocessing — no `#pragma push_macro` / `pop_macro`, `__COUNTER__`, or compiler extensions.
- **Arbitrary C / C++ header nesting (`#include` slot stack):** headers that use short macros internally can pair `<do_let_is/all_short.h>` (C++) or `<do_let_is/c/all_short.h>` (C) at the top with `<do_let_is/undef.h>` at the bottom. Macro state is pushed and popped through a Boost.Preprocessor-style `#include`-evaluated bitstack (`detail_eval_state.h`), so chains like `C++ header -> C header -> C header -> C++ header` work seamlessly with `DO` usable both before and after nested `#include`s at every level.
- **Boost.Preprocessor-style `#include` counter:** `<do_let_is/counter.h>` defines `DO_LET_IS_COUNTER` (starting at `0`), and each `#include <do_let_is/update_counter.h>` (or `#include DO_LET_IS_UPDATE_COUNTER()`) increments `DO_LET_IS_COUNTER` in pure ISO preprocessor without `__COUNTER__` or pragmas.
- **Opt-in Clang `-fblocks` backend for C++:** include `<do_let_is/all_blocks_short.h>` (or `#define DO_LET_IS_USE_BLOCKS` before `#include <do_let_is/all_short.h>`, with `-fblocks -lBlocksRuntime`) to run `DO` on Clang blocks + statement expressions in C++ with both C++ and C monads. `DO_LET_IS_USE_BLOCKS` is also stack-scoped per header and composes freely inside nested `C++ -> C -> C++(blocks) -> C++` chains.


# Examples

Without DSL:
```cpp
auto result = doletis_bind(mx, [&](auto x) {
  return doletis_bind(my, [&](auto y) {
    return make_value(x, y);
  });
});
```
With DSL:
```cpp
auto result = DO(
  LET x IS(mx);
  LET y IS(my);
  RETURN(x + y);
);
```

### Type transitions (`int` -> `double` -> `struct`)

```cpp
struct summary {
  double total;
  std::string_view tag;
  constexpr bool operator==(const summary&) const = default;
};

constexpr auto build_summary(std::optional<int> count, std::optional<double> factor) {
  return DO(
    LET n IS(count);       // int
    LET k IS(factor);      // double
    RETURN(summary{.total = static_cast<double>(n) * k, .tag = "ok"});  // std::optional<summary>
  );
}

static_assert(build_summary(12, 3.5) == summary{.total = 42.0, .tag = "ok"});
```

### Allocation-free C++ Generators (`GENERATOR` & `GENERATOR_LOOPHOLES`)

Both variants erase the continuation lambda produced at each `YIELD(...)` into a fixed-size inline buffer (`Capacity` passed to `end<T, Capacity>()`, zero heap allocations at runtime) and work in `constexpr`, but differ in how type erasure and resumption dispatch are implemented:

- **`GENERATOR` (`<do_let_is/generator.h>`):** wraps each continuation in a `constexpr virtual` interface (`vptr` + indirect virtual call on `++it`, move, and destroy; `end<T, Capacity>()` adds `sizeof(void*)` for the `vptr` automatically).
- **`GENERATOR_LOOPHOLES` (`<do_let_is/generator_loopholes.h>`):** uses friend-injection ("type loopholes" / stateful template metaprogramming) to collect the closed compile-time list of every continuation lambda type instantiated inside `impl()`. At runtime (`if !consteval`) it stores the raw lambda without a `vptr` plus an integer state ID, dispatching `++it`, move, and destroy through a closed branch/jump-table over the known concrete lambda types instead of indirect virtual calls — enabling full devirtualization and inlining across `YIELD` points like a hand-written state machine (while falling back to `constexpr virtual` inside `if consteval` so `constexpr` evaluation still works).

```cpp
#include <do_let_is/generator.h>
#include <do_let_is/generator_loopholes.h>
#include <do_let_is/short.h>

#include <algorithm>
#include <array>

constexpr auto my_generator() {
  return GENERATOR((int i), (.i = 0),
    YIELD(42);
    WHILE(i != 10)(
      YIELD(i);
      ++i;
    )
    return end<int, 16>();
  );
}

constexpr auto my_loophole_generator() {
  return GENERATOR_LOOPHOLES((int i), (.i = 0),
    YIELD(42);
    WHILE(i != 10)(
      YIELD(i);
      ++i;
    )
    return end<int, 16>();
  );
}

static_assert(std::ranges::equal(my_generator(), std::array{42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
static_assert(std::ranges::equal(my_loophole_generator(), std::array{42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
```

### Defer, Pure, and Async

`RETURN(v)` deduces the active monad from `LET ... IS(...)` and wraps `v` via `pure`:

```cpp
#include <do_let_is/async.h>
#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

doletis::executor ex;
auto [t1, p1] = doletis::task<int>::pending();
auto [t2, p2] = doletis::task<double>::pending();

auto job = DO(
  DEFER(cleanup_all());
  ERRDEFER(rollback_on_cancel_or_err());
  LET a IS(t1);       // int
  LET b IS(t2);       // double
  RETURN(a + b);      // deduced as doletis::task<double>
);

p1.resolve(20, &ex);
p2.resolve(22.0, &ex);
auto out = std::move(job).await(&ex);  // std::optional{42.0}
```

### C23 Defer, Generator, and Async

```c
#include <do_let_is/all_short.h>

DOLETIS_DEFINE_EXPECTED(int, int, res_int)
DOLETIS_DEFINE_GENERATOR(int, gen_int)
DOLETIS_DEFINE_TASK(int, task_int)

static res_int compute(res_int a, res_int b, int* trace) {
  return DO(
    DEFER(*trace += 1);
    ERRDEFER(*trace += 100);
    LET x IS(a);
    LET y IS(b);
    RETURN(x + y);
  );
}

static gen_int counter(int limit) {
  return GENERATOR(gen_int,
    __block int i = 0;
    WHILE(i < limit)(
      YIELD(i);
      ++i;
    )
    RETURN(gen_int_end());
  );
}
```

### Other examples

```cpp
#include <optional>
#include <ranges>
#include <variant>
#include <vector>

template <typename T>
struct doletis::monad_traits<std::optional<T>> {
  template <typename Opt, typename F>
  static constexpr auto bind(Opt&& value, F&& f) {
    return std::forward<Opt>(value).and_then(std::forward<F>(f));
  }
};

constexpr auto optional_function(const std::optional<int>& a, const std::optional<int>& b) {
  return DO(
      LET a IS(a);
      LET b IS(b);
      RETURN(a + b);
  );
}

template <typename T, typename Alloc>
struct doletis::monad_traits<std::vector<T, Alloc>> {
  template <typename F>
  static constexpr auto bind(const std::vector<T, Alloc>& range, F&& f) -> std::invoke_result_t<F, T> {
    return range
           | std::views::transform(std::forward<F>(f))
           | std::views::join
           | std::ranges::to<std::vector>();
  }
};

constexpr auto list_function(const std::vector<int>& a, const std::vector<int>& b) {
  return DO(
      LET a IS(a);
      LET b IS(b);
      return std::vector{a + b, a + b};
  );
}

static_assert(list_function({1, 2, 3}, {1, 2, 3}) == std::vector{2, 2, 3, 3, 4, 4, 3, 3, 4, 4, 5, 5, 4, 4, 5, 5, 6, 6});

template <typename... Ts>
struct doletis::monad_traits<std::variant<Ts...>> {
  template <typename Var, typename F>
  static constexpr auto bind(Var&& var, F&& f) {
    return std::visit(std::forward<F>(f), std::forward<Var>(var));
  }
};

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
    RETURN(result);
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
