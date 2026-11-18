#ifndef EXPECTED_H
#define EXPECTED_H

#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <expected>
#include <utility>

template <typename T, typename Err>
struct doletis::monad_traits<std::expected<T, Err>> {
  template <typename Exp, typename F>
  static constexpr auto bind(Exp&& value, F&& f) {
    return std::forward<Exp>(value).and_then(std::forward<F>(f));
  }
};

#endif
