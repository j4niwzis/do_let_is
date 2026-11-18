#ifndef NESTED_C_2_H
#define NESTED_C_2_H

#include <do_let_is/c/all_short.h>

DOLETIS_DEFINE_OPTIONAL(int, nested_c2_opt)

// C DO before nested C++-with-blocks header
static inline DO_LET_IS_UNUSED nested_c2_opt nested_level3_c_before(int v) {
  return DO(
    LET x IS(nested_c2_opt_some(v));
    RETURN(x + 3);
  );
}

// Nested C++-with-blocks header -> which itself includes pure constexpr C++ header!
#include "nested_cxx_blocks.h"

// C DO restored automatically after nested C++-with-blocks header!
static inline DO_LET_IS_UNUSED nested_c2_opt nested_level3_c_after(int v, int* defer_cnt) {
  return DO(
    DEFER(*defer_cnt += 1);
    LET x IS(nested_c2_opt_some(v));
    LET y IS(nested_level3_c_before(x));
    auto inner = nested_cxx_blocks_after(0, defer_cnt);
    RETURN(y + (inner.has_value() ? *inner : -999));
  );
}

#include <do_let_is/undef.h>

#endif
