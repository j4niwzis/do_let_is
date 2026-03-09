#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <do_let_is/short.h>

#include <optional>

namespace doletis {

template <typename T, typename F>
constexpr auto bind(const std::optional<T>& value, F&& f) {
  return value.and_then(std::forward<F>(f));
}

template <typename T, typename F>
constexpr auto bind(std::optional<T>& value, F&& f) {
  return value.and_then(std::forward<F>(f));
}

template <typename T, typename F>
constexpr auto bind(std::optional<T>&& value, F&& f) {
  return std::move(value).and_then(std::forward<F>(f));
}

}  // namespace doletis

#endif
