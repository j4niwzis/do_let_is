#ifndef TYPE_SET_H
#define TYPE_SET_H
#include <compare>
#include <utility>

namespace doletis {

template <typename... Ts>
struct type_set {};

template <typename... Ts, typename... Ts2>
constexpr auto operator+(type_set<Ts...>, type_set<Ts2...>) -> type_set<Ts..., Ts2...> {
  return {};
}

namespace internal {

struct dont_use_me {
  static consteval void test();
};

template <typename T = dont_use_me, typename Base = dont_use_me>
struct overload_check : Base {
  using Base::test;
  static consteval void test(type_set<T> value)
    requires true;
  static consteval void test(type_set<T> value)
    requires(requires { Base::test(value); });

  template <typename T2>
  constexpr auto operator|(type_set<T2>) {
    constexpr auto next = overload_check<T2, overload_check>{};
    if constexpr (requires { next.test(type_set<T2>{}); }) {
      return next;
    } else {
      return *this;
    }
  }
};

template <typename T>
struct overload_check_to_type_set {};

template <typename T, typename Base>
struct overload_check_to_type_set<overload_check<T, Base>> {
  using type = decltype(type_set<T>{} + typename overload_check_to_type_set<Base>::type{});
};

template <>
struct overload_check_to_type_set<overload_check<dont_use_me, dont_use_me>> {
  using type = type_set<>;
};

template <typename... Ts>
struct get_unique {
  using type = overload_check_to_type_set<decltype((overload_check<>{} | ... | type_set<Ts>{}))>::type;
};

template <typename T, auto>
struct callable {
  int operator()(type_set<T>) const;
};

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

consteval void to_void(...);

}  // namespace internal

template <typename... Ts, typename... Ts2>
consteval auto operator>=(type_set<Ts...>, type_set<Ts2...>) {
  constexpr auto a = []<typename... Ts3, std::size_t... Is>(type_set<Ts3...>, std::index_sequence<Is...>) {
    return internal::overloaded<internal::callable<Ts3, Is>...>{};
  }(typename internal::get_unique<Ts...>::type{}, std::index_sequence_for<Ts...>{});

  return []<typename... Ts4>(type_set<Ts4...>) {
    return requires { internal::to_void(a(type_set<Ts4>{})...); };
  }(typename internal::get_unique<Ts2...>::type{});
}

template <typename... Ts, typename... Ts2>
consteval auto operator<=(type_set<Ts...> a, type_set<Ts2...> b) {
  return b >= a;
}

template <typename... Ts, typename... Ts2>
consteval auto operator==(type_set<Ts...> a, type_set<Ts2...> b) {
  return a >= b && a <= b;
}

template <typename... Ts, typename... Ts2>
consteval std::partial_ordering operator<=>(type_set<Ts...> a, type_set<Ts2...> b) {
  auto flag1 = a >= b;
  auto flag2 = a <= b;
  return flag1 && flag2 ? std::partial_ordering::equivalent
         : flag1        ? std::partial_ordering::greater
         : flag2        ? std::partial_ordering::less
                        : std::partial_ordering::unordered;
}

}  // namespace doletis

#endif
