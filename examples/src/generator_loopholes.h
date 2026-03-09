#ifndef GENERATOR_LOOPHOLES

#include <do_let_is.h>

#include <optional>

#include "generator.h"
#ifndef GENERATOR
#endif
#include "inplace_function_loopholes.h"

#undef LAMBDA_CAPTURE
#define LAMBDA_CAPTURE /* NOLINT */ =, this

#define GENERATOR_LOOPHOLES(fields, init, ...)                                                                \
  [&] {                                                                                                       \
    struct : ::doletis::generator_base {                                                                      \
      struct tag {};                                                                                          \
      UNWRAP fields;                                                                                          \
      constexpr auto impl() {                                                                                 \
        EVAL(PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__)));                                                   \
        static_assert((std::ignore = ::stdext::loopholes::define_function<tag, decltype(impl())()>{}, true)); \
      }                                                                                                       \
                                                                                                              \
    } gen{UNWRAP init};                                                                                       \
    return gen;                                                                                               \
  }()

#endif
