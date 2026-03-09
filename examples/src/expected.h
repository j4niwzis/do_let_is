#ifndef EXPECTED_H
#define EXPECTED_H
#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <expected>

namespace doletis {

template <typename T, typename Err, typename F>
constexpr auto bind(const std::expected<T, Err>& value, F&& f) {
  return value.and_then(std::forward<F>(f));
}

template <typename T, typename Err, typename F>
constexpr auto bind(std::expected<T, Err>& value, F&& f) {
  return value.and_then(std::forward<F>(f));
}

template <typename T, typename Err, typename F>
constexpr auto bind(std::expected<T, Err>&& value, F&& f) {
  return std::move(value).and_then(std::forward<F>(f));
}

}  // namespace doletis

#endif
