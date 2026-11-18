#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <optional>
#include <utility>

template <typename T>
struct doletis::monad_traits<std::optional<T>> {
  template <typename Opt, typename F>
  static constexpr auto bind(Opt&& value, F&& f) {
    return std::forward<Opt>(value).and_then(std::forward<F>(f));
  }
};

#endif
