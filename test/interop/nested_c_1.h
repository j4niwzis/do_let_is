#ifndef NESTED_C_1_H
#define NESTED_C_1_H

#include <do_let_is/c/all_short.h>

DOLETIS_DEFINE_OPTIONAL(int, nested_c1_opt)

// C DO before nested C header
static inline DO_LET_IS_UNUSED nested_c1_opt nested_level2_c_before(int v) {
  return DO(
    LET x IS(nested_c1_opt_some(v));
    RETURN(x + 2);
  );
}

// Second C header inside first C header!
#include "nested_c_2.h"

// C DO still active after nested C header!
static inline DO_LET_IS_UNUSED nested_c1_opt nested_level2_c_after(int v, int* defer_cnt) {
  return DO(
    LET x IS(nested_level2_c_before(v));
    nested_c2_opt inner = nested_level3_c_after(x, defer_cnt);
    LET y IS(inner.has_value ? nested_c1_opt_some(inner.value) : nested_c1_opt_none());
    RETURN(y);
  );
}

#include <do_let_is/undef.h>

#endif
