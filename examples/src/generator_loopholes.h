#ifndef GENERATOR_LOOPHOLES

#include <do_let_is/short.h>

#include "generator.h"
#ifndef GENERATOR
#endif

#include "inplace_function_loopholes.h"

#ifndef INPLACE_FUNCTION_LOOPHOLES_H
#endif

#undef DO_LET_IS_LAMBDA_CAPTURE
#define DO_LET_IS_LAMBDA_CAPTURE /* NOLINT */ =, this

#define GENERATOR_LOOPHOLES(fields, init, ...)                                                                \
  [&] {                                                                                                       \
    struct : ::doletis::generator_base {                                                                      \
      struct tag {};                                                                                          \
      DO_LET_IS_UNWRAP fields;                                                                                \
      constexpr auto impl() {                                                                                 \
        DO_LET_IS_EVAL(DO_LET_IS_PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__)));                               \
        static_assert((std::ignore = ::stdext::loopholes::define_function<tag, decltype(impl())()>{}, true)); \
      }                                                                                                       \
    } gen{DO_LET_IS_UNWRAP init};                                                                             \
    return gen;                                                                                               \
  }()

#endif
