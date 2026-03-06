#ifndef GENERATOR

#include <do_let_is.h>

#include <iterator>
#include <optional>

#include "generator_continuation_loopholes.h"
#include "inplace_function.h"

#undef LAMBDA_CAPTURE
#define LAMBDA_CAPTURE /* NOLINT */ =, this

#define GENERATOR(fields, init, ...)                                               \
  [&] {                                                                            \
    struct : ::doletis::generator_base {                                           \
      UNWRAP fields;                                                               \
      constexpr auto impl() { EVAL(PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))) } \
    } gen{UNWRAP init};                                                            \
    return gen;                                                                    \
  }()

#define YIELD(...) LET _ IS(::doletis::yielder{__VA_ARGS__})

namespace doletis {

template <typename T>
struct yielder {
  T value;
};

template <typename T, std::size_t Capacity,
          std::size_t Alignment = 8>  // NOLINT
struct generator_continuation {
  using value_type = T;
  struct type {
    T value;
    stdext::inplace_function<generator_continuation(), Capacity, Alignment> f;
  };
  std::optional<type> value = std::nullopt;
};

template <typename T, typename F>
constexpr auto bind(yielder<T> value, F&& f) {
  using R = decltype(std::forward<F>(f)(std::monostate{}));
  return R{.value = typename R::type{.value = std::move(value.value),
                                     .f = [f = std::forward<F>(f)] mutable { return std::move(f)(std::monostate{}); }}};
}

struct generator_base {
  template <typename S>
  struct iterator {
    using step_t = decltype(std::declval<S&>().impl());
    using value_type = step_t::value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_concept = std::input_iterator_tag;
    step_t current;

    constexpr value_type operator*() const { return current.value->value; }

    constexpr iterator& operator++() {
      current = current.value->f();
      return *this;
    }

    constexpr void operator++(int) { ++(*this); }

    constexpr bool operator==(std::default_sentinel_t) const { return !current.value; }
  };
  template <typename S>
  constexpr iterator<S> begin(this const S& s) = delete;
  template <typename S>
  constexpr iterator<S> begin(this S& s) {
    return iterator<S>{s.impl()};
  }

  constexpr std::default_sentinel_t end() const noexcept { return std::default_sentinel; }

  template <typename T, std::size_t Capacity, std::size_t Alignment = sizeof(void*), typename Self>
  constexpr auto end(this const Self& self) {
    if constexpr (requires { typename std::decay_t<Self>::tag; }) {
      return loopholes::generator_continuation<typename std::decay_t<Self>::tag, T, Capacity, Alignment>{};
    } else {
      return generator_continuation<T, Capacity + sizeof(void*), Alignment>{};
    }
  }
};
}  // namespace doletis

#endif
