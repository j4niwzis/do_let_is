#ifndef VECTOR_H
#define VECTOR_H

#include <do_let_is/do_let_is.h>
#include <do_let_is/short.h>

#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T, typename Alloc>
struct doletis::monad_traits<std::vector<T, Alloc>> {
  template <typename F>
  static constexpr auto bind(const std::vector<T, Alloc>& range, F&& f) -> std::invoke_result_t<F, T> {
    return range                                        //
           | std::views::transform(std::forward<F>(f))  //
           | std::views::join                           //
           | std::ranges::to<std::vector>();
  }
};

#endif
