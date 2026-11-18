#ifndef SAMPLE_C_API1_H
#define SAMPLE_C_API1_H

#include <do_let_is/c/all_short.h>

DOLETIS_DEFINE_OPTIONAL(int, c_api1_opt)

static inline DO_LET_IS_UNUSED c_api1_opt c_api1_add(c_api1_opt a, c_api1_opt b) {
  return DO(
    LET x IS(a);
    LET y IS(b);
    RETURN(x + y);
  );
}

#include <do_let_is/undef.h>

#endif
