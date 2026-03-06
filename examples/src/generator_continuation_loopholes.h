#ifndef GENERATOR_CONTINUATION_LOOPHOLES_H
#define GENERATOR_CONTINUATION_LOOPHOLES_H

#include <cstddef>
#include <optional>

#include "inplace_function_loopholes.h"

namespace doletis::loopholes {

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
