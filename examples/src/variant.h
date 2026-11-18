#ifndef VARIANT_H
#define VARIANT_H

#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <utility>
#include <variant>

template <typename... Ts>
struct doletis::monad_traits<std::variant<Ts...>> {
  template <typename Var, typename F>
  static constexpr auto bind(Var&& var, F&& f) {
    return std::visit(std::forward<F>(f), std::forward<Var>(var));
  }
};

#endif
