#ifndef DO_LET_IS_GENERATOR_LOOPHOLES_H
#define DO_LET_IS_GENERATOR_LOOPHOLES_H

#include "do_let_is/generator.h"
#include "do_let_is/inplace_function_loopholes.h"

#undef DO_LET_IS_LAMBDA_CAPTURE
#define DO_LET_IS_LAMBDA_CAPTURE /* NOLINT */ =, this

#define DO_LET_IS_GENERATOR_LOOPHOLES(fields, init, ...)                                                       \
  [&] {                                                                                                        \
    struct : ::doletis::generator_base {                                                                       \
      struct tag {};                                                                                           \
      DO_LET_IS_UNWRAP fields;                                                                                 \
      constexpr auto impl() {                                                                                  \
        DO_LET_IS_EVAL(DO_LET_IS_PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__)));                                \
        static_assert((std::ignore = ::doletis::loopholes::define_function<tag, decltype(impl())()>{}, true)); \
      }                                                                                                        \
    } gen{DO_LET_IS_UNWRAP init};                                                                              \
    return gen;                                                                                                \
  }()

#endif
