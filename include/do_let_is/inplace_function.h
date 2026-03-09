#ifndef DO_LET_IS_INPLACE_FUNCTION_H
#define DO_LET_IS_INPLACE_FUNCTION_H
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace doletis {

namespace internal {

template <typename R, typename... Args>
struct interface {
  constexpr virtual ~interface() = default;
  constexpr virtual R operator()(Args... args) = 0;
  constexpr virtual interface* move_to(void* ptr) noexcept = 0;
};

template <typename F, typename R, typename... Args>
struct concrete : interface<R, Args...> {
  F func;
  explicit constexpr concrete(F f) : func(std::move(f)) {}
  constexpr R operator()(Args... args) override { return func(std::forward<Args>(args)...); }
  constexpr interface<R, Args...>* move_to(void* ptr) noexcept override {
    if consteval {
      return new concrete{std::move(this->func)};  // NOLINT
    }
    return new (ptr) concrete{std::move(this->func)};  // NOLINT
  }
};

}  // namespace internal

template <typename Signature, std::size_t Capacity, std::size_t Alignment>
struct inplace_function;

template <typename R, typename... Args, std::size_t Capacity, std::size_t Alignment>
struct inplace_function<R(Args...), Capacity, Alignment> {
  using interface = internal::interface<R, Args...>;

  template <typename F>  // NOLINTNEXTLINE
  constexpr inplace_function(F&& f) : _ptr(create(std::forward<F>(f))) {}

  // NOLINTNEXTLINE
  constexpr inplace_function(inplace_function&& other) noexcept : _ptr(other._ptr->move_to(_data)) {}
  constexpr inplace_function(const inplace_function&) = delete;
  constexpr inplace_function& operator=(inplace_function&& other) noexcept {
    std::destroy_at(this);
    std::construct_at(this, std::move(other));
    return *this;
  }
  constexpr auto& operator=(const inplace_function&) = delete;
  constexpr ~inplace_function() {
    if consteval {
      delete _ptr;
      return;
    }
    std::destroy_at(_ptr);
  }
  constexpr R operator()(Args... args) { return (*_ptr)(std::forward<Args>(args)...); }

 private:
  template <typename F>
  constexpr interface* create(F&& f) {
    using T = internal::concrete<std::decay_t<F>, R, Args...>;
    static_assert(sizeof(T) <= Capacity);
    static_assert(Alignment % alignof(T) == 0);
    if consteval {
      return new T{std::forward<F>(f)};  // NOLINT
    }
    return new (_data) T{std::forward<F>(f)};  // NOLINT
  }
  interface* _ptr;
  alignas(Alignment) std::byte _data[Capacity];  // NOLINT
};

}  // namespace doletis

#endif
