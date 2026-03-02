#ifndef GENERATOR

#include <iterator>
#include <variant>

#include "do_let_is.h"
#include "inplace_function.h"

#undef LAMBDA_CAPTURE
#define LAMBDA_CAPTURE =, this

#define GENERATOR(fields, init, ...)                        \
  [&] {                                                     \
    struct : generator_base {                               \
      UNWRAP fields;                                        \
      auto impl() {                                         \
        EVAL(PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))); \
      }                                                     \
    } gen{UNWRAP init};                                     \
    return gen;                                             \
  }()

#define YIELD(...) LET _ IS(yielder{__VA_ARGS__})

template <typename T>
struct yielder {
  T value;
};

template <typename T, std::size_t Capacity, std::size_t Alignment = 8>  // NOLINT
struct rec {
  using value_type = T;
  struct Some {
    T value;
    stdext::inplace_function<rec(), Capacity, Alignment> f;
  };
  std::variant<std::monostate, Some> value;
};

template <typename T, typename F>
constexpr auto bind(yielder<T> value, F&& f) {
  using R = decltype(std::forward<F>(f)(std::monostate{}));
  return R{.value = typename R::Some{.value = std::move(value.value), .f = [f = std::forward<F>(f)] mutable {
                                       return std::move(f)(std::monostate{});
                                     }}};
}

struct generator_base {
  template <typename S>
  struct iterator {
    using step_t = decltype(std::declval<S&>().impl());
    using value_type = step_t::value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_concept = std::input_iterator_tag;
    step_t current;

    constexpr auto operator*() const {
      return std::get<1>(current.value).value;
    }

    constexpr iterator& operator++() {
      current = std::get<1>(current.value).f();
      return *this;
    }

    constexpr void operator++(int) {
      ++(*this);
    }

    constexpr bool operator==(std::default_sentinel_t) const {
      return current.value.index() == 0;
    }
  };
  template <typename S>
  constexpr iterator<S> begin(this const S& s) = delete;
  template <typename S>
  constexpr iterator<S> begin(this S& s) {
    return iterator<S>{s.impl()};
  }

  constexpr std::default_sentinel_t end() const noexcept {
    return std::default_sentinel;
  }
};

#endif
