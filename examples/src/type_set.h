#ifndef TYPE_SET_H
#define TYPE_SET_H
#include <compare>
#include <utility>

namespace doletis {

template <typename... Ts>
struct type_set {
  static constexpr std::size_t size = sizeof...(Ts);
};

template <typename... Ts, typename... Ts2>
constexpr auto operator+(type_set<Ts...>, type_set<Ts2...>) -> type_set<Ts..., Ts2...> {
  return {};
}

namespace internal {

struct overload_check_root {
  static consteval void test();
};

template <typename T = overload_check_root, typename Base = overload_check_root>
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
struct overload_check_to_type_set<overload_check<overload_check_root, overload_check_root>> {
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
  using ulist = typename internal::get_unique<Ts...>::type;
  constexpr auto a = []<typename... Ts3, std::size_t... Is>(type_set<Ts3...>, std::index_sequence<Is...>) {
    return internal::overloaded<internal::callable<Ts3, Is>...>{};
  }(ulist{}, std::make_index_sequence<ulist::size>{});

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

template <typename... Ts>
consteval bool is_unique(type_set<Ts...> obj = {}) {
  constexpr auto a = []<std::size_t... Is>(std::index_sequence<Is...>) {
    return internal::overloaded<internal::callable<Ts, Is>...>{};
  }(std::index_sequence_for<Ts...>{});
  return requires { internal::to_void(a(type_set<Ts>{})...); };
}

}  // namespace doletis

#endif
