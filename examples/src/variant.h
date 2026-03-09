#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <variant>

namespace doletis {

template <typename... Ts, typename F>
constexpr auto bind(const std::variant<Ts...>& var, F&& f) {
  return std::visit(std::forward<F>(f), var);
}

}  // namespace doletis
