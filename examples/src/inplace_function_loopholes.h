#ifndef INPLACE_FUNCTION_LOOPHOLES_H
#define INPLACE_FUNCTION_LOOPHOLES_H
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

#include "inplace_function.h"
#include "loopholes.h"

namespace stdext {

namespace loopholes {

template <typename Tag, typename Signature>
struct declare_function {};

template <typename Tag, typename Signature>
struct define_function {};

template <typename Tag, typename R, typename... Args>
struct declare_function<Tag, R(Args...)> {
  friend inline void* move_to(declare_function, void* ptr, std::size_t id, void* to);
  friend inline R call(declare_function, void* ptr, std::size_t id, Args...);
  friend inline void destroy(declare_function, void* ptr, std::size_t id);
};

template <typename Tag, typename Signature, std::size_t Capacity, std::size_t Alignment>
struct inplace_function_runtime;

template <typename Tag, typename R, typename... Args, std::size_t Capacity, std::size_t Alignment>
struct inplace_function_runtime<Tag, R(Args...), Capacity, Alignment> {
  using tag = Tag;
  using call_tag = declare_function<Tag, R(Args...)>;
  template <typename F, auto Id = add_to_set<Tag, std::decay_t<F>>()>
  inline inplace_function_runtime(F&& f) : _ptr(create(std::forward<F>(f))), _id(Id) {  // NOLINT
  }

  inline inplace_function_runtime(inplace_function_runtime&& other) noexcept  // NOLINT
      : _ptr(move_to(call_tag{}, other._ptr, other._id, _data)), _id(other._id) {}
  inplace_function_runtime(const inplace_function_runtime&) = delete;
  inline inplace_function_runtime& operator=(inplace_function_runtime&& other) noexcept {
    if (&other == this) {
      return *this;
    }
    std::destroy_at(this);
    std::construct_at(this, std::move(other));
    return *this;
  }
  auto& operator=(const inplace_function_runtime&) = delete;
  inline ~inplace_function_runtime() { destroy(call_tag{}, _ptr, _id); }
  inline R operator()(Args... args) { return call(call_tag{}, _ptr, _id, std::forward<Args>(args)...); }

  template <typename F>
  inline void* create(F&& f) {
    using T = std::decay_t<F>;
    static_assert(sizeof(T) <= Capacity);
    static_assert(Alignment % alignof(T) == 0);

    return new (_data) T{std::forward<F>(f)};  // NOLINT
  }

 private:
  void* _ptr;
  std::size_t _id;
  alignas(Alignment) std::byte _data[Capacity];  /// NOLINT
};

struct deduct {};

template <typename R = deduct, std::size_t N = 0, typename Visitor, typename... Ts>
inline auto visit_ptr(Visitor&& visitor, void* ptr, std::size_t id, type_list<Ts...> list) {
  using tup = std::tuple<std::type_identity<Ts>...>;
  if constexpr (N == sizeof...(Ts)) {
    return [] -> R { std::unreachable(); }();
  } else {
    auto fn = [&] {
      auto fn = [&] {
        return std::forward<Visitor>(visitor)(static_cast<std::decay_t<decltype(std::get<N>(std::declval<tup>()))>::type*>(ptr));
      };
      if constexpr (std::is_same_v<R, deduct>) {
        return fn();
      } else {
        return [&] -> R { return fn(); }();
      }
    };
    using result = std::conditional_t<std::is_same_v<R, deduct>, decltype(fn()), R>;
    if (id == N) {
      return fn();
    } else {
      return visit_ptr<result, N + 1>(std::forward<Visitor>(visitor), ptr, id, list);
    }
  }
}

template <typename Tag, typename R, typename... Args>
struct define_function<Tag, R(Args...)> {
  using call_tag = declare_function<Tag, R(Args...)>;
  template <typename Visitor>
  static inline auto visit(Visitor&& visitor, void* ptr, std::size_t id) {
    return visit_ptr(std::forward<Visitor>(visitor), ptr, id, read_set<Tag>());
  }
  friend inline void* move_to(call_tag tag, void* ptr, std::size_t id, void* to) {
    return visit([&]<typename T>(T* ptr) -> void* { return new (to) T{std::move(*ptr)}; }, ptr, id);
  }
  friend inline R call(call_tag tag, void* ptr, std::size_t id, Args... args) {
    return visit([&](auto ptr) -> R { return (*ptr)(std::forward<Args>(args)...); }, ptr, id);
  }
  friend inline void destroy(call_tag tag, void* ptr, std::size_t id) {
    visit([]<typename T>(T* ptr) { ptr->~T(); }, ptr, id);
  }
};

template <typename Tag, typename Signature, std::size_t Capacity, std::size_t Alignment>
struct inplace_function;

template <typename Tag, typename R, typename... Args, std::size_t Capacity, std::size_t Alignment>
struct inplace_function<Tag, R(Args...), Capacity, Alignment> {
  template <typename F, auto Id = add_to_set<Tag, std::decay_t<F>>()>
  constexpr inplace_function(F&& f) {  // NOLINT
    if consteval {
      using T = internal::concrete<std::decay_t<F>, R, Args...>;
      std::construct_at(&ct, new T{std::forward<F>(f)});  // NOLINT
    } else {
      std::construct_at(&rt, std::forward<F>(f));  // NOLINT
    }
  }
  constexpr R operator()(Args... args) {
    if consteval {
      return (*ct)(std::forward<Args>(args)...);  // NOLINT
    } else {
      return rt(std::forward<Args>(args)...);  // NOLINT
    }
  }
  constexpr inplace_function(inplace_function&& other) noexcept {
    if consteval {
      std::construct_at(&ct, std::move(other.ct));  // NOLINT
    } else {
      std::construct_at(&rt, std::move(other.rt));  // NOLINT
    }
  }
  constexpr inplace_function(const inplace_function&) = delete;
  constexpr inplace_function& operator=(inplace_function&& other) noexcept {
    if (&other == this) {
      return *this;
    }
    std::destroy_at(this);
    std::construct_at(this, std::move(other));
    return *this;
  }
  constexpr auto operator=(const inplace_function&) = delete;
  constexpr ~inplace_function() {
    if consteval {
      std::destroy_at(&ct);  // NOLINT
    } else {
      std::destroy_at(&rt);  // NOLINT
    }
  }

 private:
  union {
    inplace_function_runtime<Tag, R(Args...), Capacity, Alignment> rt;
    std::unique_ptr<internal::interface<R, Args...>> ct;
  };
};

}  // namespace loopholes
}  // namespace stdext

#endif
