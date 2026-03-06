#ifndef GENERATOR_LOOPHOLES

#include <do_let_is.h>

#include <optional>

#include "generator.h"
#ifndef GENERATOR
#endif
#include "inplace_function_loopholes.h"

#undef LAMBDA_CAPTURE
#define LAMBDA_CAPTURE /* NOLINT */ =, this

#define GENERATOR_LOOPHOLES(fields, init, ...)                                                              \
  [&] {                                                                                                     \
    struct : ::doletis::generator_base {                                                                    \
      struct tag {};                                                                                        \
      UNWRAP fields;                                                                                        \
      constexpr auto impl() {                                                                               \
        EVAL(PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__)));                                                 \
        static_assert((std::ignore = stdext::loopholes::define_function<tag, decltype(impl())()>{}, true)); \
      }                                                                                                     \
                                                                                                            \
    } gen{UNWRAP init};                                                                                     \
    return gen;                                                                                             \
  }()

namespace doletis::loopholes {

template <typename T>
struct yielder {
  T value;
};

template <typename Tag, typename T, std::size_t Capacity,
          std::size_t Alignment = 8>  // NOLINT
struct generator_continuation {
  using value_type = T;
  struct type {
    T value;
    stdext::loopholes::inplace_function<Tag, generator_continuation(), Capacity, Alignment> f;
  };
  std::optional<type> value = std::nullopt;
};

}  // namespace doletis::loopholes

#endif
