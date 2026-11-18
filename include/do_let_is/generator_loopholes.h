#ifndef DO_LET_IS_CXX_GENERATOR_LOOPHOLES_H
#define DO_LET_IS_CXX_GENERATOR_LOOPHOLES_H

#include "do_let_is/generator.h"
#include "do_let_is/generator_continuation_loopholes.h"
#include "do_let_is/inplace_function_loopholes.h"

#define DO_LET_IS_CXX_GENERATOR_LOOPHOLES(fields, init, ...)                                                   \
  [&] {                                                                                                        \
    struct : ::doletis::generator_base {                                                                       \
      struct tag {};                                                                                           \
      DO_LET_IS_UNWRAP fields;                                                                                 \
      constexpr auto impl() {                                                                                  \
        using _do_monad_t [[maybe_unused]] = void;                                                             \
        DO_LET_IS_EVAL(DO_LET_IS_CXX_PARSE_DO_ITERATION(0, 0, 0, 1, 1, _CODE(__VA_ARGS__)));                      \
        static_assert((std::ignore = ::doletis::loopholes::define_function<tag, decltype(impl())()>{}, true)); \
      }                                                                                                        \
    } gen{DO_LET_IS_UNWRAP init};                                                                              \
    return gen;                                                                                                \
  }()

#define DO_LET_IS_HAVE_CXX_GENERATOR_LOOPHOLES 1

#endif  // DO_LET_IS_CXX_GENERATOR_LOOPHOLES_H

#ifndef DO_LET_IS_GENERATOR_LOOPHOLES
#define DO_LET_IS_GENERATOR_LOOPHOLES DO_LET_IS_CXX_GENERATOR_LOOPHOLES
#endif
