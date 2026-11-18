#ifndef SAMPLE_C_API2_H
#define SAMPLE_C_API2_H

#include <do_let_is/c/all_short.h>

DOLETIS_DEFINE_EXPECTED(int, int, c_api2_res)

static inline DO_LET_IS_UNUSED c_api2_res c_api2_txn(bool fail, int* trace) {
  return DO(
    DEFER(*trace = *trace * 10 + 1);
    ERRDEFER(*trace = *trace * 10 + 9);
    LET x IS(c_api2_res_ok(20));
    LET y IS(fail ? c_api2_res_err(-5) : c_api2_res_ok(22));
    RETURN(x + y);
  );
}

#include <do_let_is/undef.h>

#endif
