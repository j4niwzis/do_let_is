#ifndef ERRORED_H
#define ERRORED_H
#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <functional>
#include <variant>

namespace doletis {

template <typename... Ts>
struct type_list {};

template <typename... Ts, typename... Ts2>
constexpr auto operator+(type_list<Ts...>, type_list<Ts2...>) -> type_list<Ts..., Ts2...> {
  return {};
}

namespace internal {

struct dont_use_me {
  static consteval void test();
};

template <typename T = dont_use_me, typename Base = dont_use_me>
struct overload_check : Base {
  using Base::test;
  static consteval void test(type_list<T> value)
    requires(!requires { Base::test(value); });
  template <typename T2>
  constexpr auto operator|(type_list<T2>) {
    constexpr auto next = overload_check<T2, overload_check>{};
    if constexpr (requires { next.test(type_list<T2>{}); }) {
      return next;
    } else {
      return *this;
    }
  }
};

template <typename T>
struct overload_check_to_type_list {};

template <typename T, typename Base>
struct overload_check_to_type_list<overload_check<T, Base>> {
  using type = decltype(type_list<T>{} + typename overload_check_to_type_list<Base>::type{});
};

template <>
struct overload_check_to_type_list<overload_check<dont_use_me, dont_use_me>> {
  using type = type_list<>;
};

template <typename... Ts>
struct get_unique {
  using type = overload_check_to_type_list<decltype((overload_check<>{} | ... | type_list<Ts>{}))>::type;
};

template <typename T, auto>
struct callable {
  int operator()(type_list<T>) const;
};

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

consteval void to_void(...);

}  // namespace internal

template <typename... Ts, typename... Ts2>
consteval auto operator>=(type_list<Ts...>, type_list<Ts2...>) {
  constexpr auto a = []<typename... Ts3, std::size_t... Is>(type_list<Ts3...>, std::index_sequence<Is...>) {
    return internal::overloaded<internal::callable<Ts3, Is>...>{};
  }(typename internal::get_unique<Ts...>::type{}, std::index_sequence_for<Ts...>{});

  return []<typename... Ts4>(type_list<Ts4...>) {
    return requires { internal::to_void(a(type_list<Ts4>{})...); };
  }(typename internal::get_unique<Ts2...>::type{});
}

template <typename... Ts, typename... Ts2>
consteval auto operator<=(type_list<Ts...> a, type_list<Ts2...> b) {
  return b >= a;
}

template <typename... Ts, typename... Ts2>
consteval auto operator==(type_list<Ts...> a, type_list<Ts2...> b) {
  return a >= b && a <= b;
}

template <typename... Ts, typename... Ts2>
consteval std::partial_ordering operator<=>(type_list<Ts...> a, type_list<Ts2...> b) {
  auto flag1 = a >= b;
  auto flag2 = a <= b;
  return flag1 && flag2 ? std::partial_ordering::equivalent
         : flag1        ? std::partial_ordering::greater
         : flag2        ? std::partial_ordering::less
                        : std::partial_ordering::unordered;
}

template <typename T>
struct error {
  T value;
};

template <typename Value, typename... Error>
struct errored;

namespace internal {

template <typename Value, typename... Error>
consteval errored<Value, Error...> get_errored(type_list<Error...>);

template <typename Value, typename... Error, typename... Error2>
consteval auto get_errored(type_list<Error...> list1, type_list<Error2...> list2)
    -> decltype(get_errored<Value>(typename get_unique<Error..., Error2...>::type{}));

template <typename Result, typename... Error>
constexpr auto convert_errored(std::variant<Error...> obj) {
  return std::visit(
      []<typename T>(T value) -> Result {  // NOLINT
        return ::doletis::error<T>{std::move(value)};
      },
      std::move(obj));
}

template <typename Errored, typename F>
constexpr auto bind_errored(Errored&& value, F&& f) {
  using errored_t = std::decay_t<Errored>;
  using value_type = errored_t::value_type;
  using invoke_result = std::invoke_result_t<F, value_type>;
  using result =
      decltype(get_errored<typename invoke_result::value_type>(typename errored_t::error_list{}, typename invoke_result::error_list{}));

  if (value.has_value()) {
    return static_cast<result>(std::invoke(std::forward<F>(f), std::forward<Errored>(value).value()));
  }
  return static_cast<result>(std::forward<Errored>(value));
}

}  // namespace internal

template <typename Value, typename... Error>
struct errored {
  using error_list = type_list<Error...>;
  using value_type = Value;
  constexpr bool has_value() const { return _value.index() == 0; }
  constexpr bool has_error() const { return !this->has_value(); }
  template <typename Self>
  constexpr decltype(auto) value(this Self&& self) {  // NOLINT
    return std::forward_like<Self>(std::get<0>(self._value));
  }
  template <typename Self>
  constexpr decltype(auto) error(this Self&& self) {  // NOLINT
    return std::forward_like<Self>(std::get<1>(self._value));
  }
  constexpr errored(Value value) : _value(std::in_place_index<0>, std::move(value)) {  // NOLINT
  }
  template <typename T>
  constexpr errored(::doletis::error<T>&& value) : _value(std::in_place_index<1>, std::move(value).value) {  // NOLINT
  }
  template <typename T>
  constexpr errored(::doletis::error<T>& value) : _value(std::in_place_index<1>, value.value) {  // NOLINT
  }

  template <typename T>
  constexpr errored(const ::doletis::error<T>& value) : _value(std::in_place_index<1>, value.value) {  // NOLINT
  }

  template <typename Self, typename... Error2>
  constexpr explicit operator errored<Value, Error2...>(this Self&& self)
    requires(type_list<Error2...>{} >= error_list{})
  {
    if (self.has_value()) {
      return std::forward<Self>(self).value();
    }
    return internal::convert_errored<errored<Value, Error2...>>(std::forward<Self>(self).error());
  }

  template <typename Self, typename Value2, typename... Error2>
  constexpr explicit operator errored<Value2, Error2...>(this Self&& self)
    requires(type_list<Error2...>{} >= error_list{})
  {
    if (!self.has_error()) {
      struct : std::exception {
        constexpr const char* what() const noexcept override { return "Invalid cast"; }
      } obj;
      throw obj;
    }
    return internal::convert_errored<errored<Value2, Error2...>>(std::forward<Self>(self).error());
  }

  template <typename Self, typename F>
  constexpr auto then(this Self&& self, F&& f) {
    return internal::bind_errored(std::forward<Self>(self), std::forward<F>(f));
  }

 private:
  std::variant<Value, std::variant<Error...>> _value;
};

template <typename Value>
struct errored<Value> {
  using value_type = Value;
  using error_list = type_list<>;
  constexpr bool has_value() const { return true; }
  constexpr bool has_error() const { return false; }

  template <typename Self>
  constexpr decltype(auto) value(this Self&& self) {  // NOLINT
    return std::forward_like<Self>(self._value);
  }

  constexpr errored(Value value) : _value(std::move(value)) {  // NOLINT
  }

  template <typename... Ts>
  constexpr explicit operator errored<Value, Ts...>() const {
    return {value()};
  }

  template <typename Self, typename F>
  constexpr auto then(this Self&& self, F&& f) {
    return internal::bind_errored(std::forward<Self>(self), std::forward<F>(f));
  }

 private:
  Value _value;
};

struct void_result {};

template <typename... Error>
struct errored<void, Error...> : errored<void_result, Error...> {
  using errored<void_result, Error...>::errored;
};

template <typename Value, typename... Error, typename F>
constexpr auto bind(const errored<Value, Error...>& value, F&& f) {
  return internal::bind_errored(value, std::forward<F>(f));
}

template <typename Value, typename... Error, typename F>
constexpr auto bind(errored<Value, Error...>& value, F&& f) {
  return internal::bind_errored(value, std::forward<F>(f));
}

template <typename Value, typename... Error, typename F>
constexpr auto bind(errored<Value, Error...>&& value, F&& f) {
  return internal::bind_errored(std::move(value), std::forward<F>(f));
}

}  // namespace doletis

#endif
