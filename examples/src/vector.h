#include <do_let_is/short.h>

#include <ranges>
#include <vector>

namespace doletis {

template <typename T, typename F>
constexpr auto bind(const std::vector<T>& range, F&& f) -> std::invoke_result_t<F, T> {
  return range                                        //
         | std::views::transform(std::forward<F>(f))  //
         | std::views::join                           //
         | std::ranges::to<std::vector>();
}

}  // namespace doletis
