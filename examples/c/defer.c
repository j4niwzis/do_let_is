#include <do_let_is/c/monads.h>
#include <do_let_is/short.h>
#include <stdlib.h>

#define CHECK(cond) \
  do {              \
    if (!(cond)) {  \
      abort();      \
    }               \
  } while (0)

DOLETIS_DEFINE_EXPECTED(int, int, res_int)

static res_int run_txn(bool fail_mid, int* trace) {
  return DO(
    DEFER(*trace = *trace * 10 + 1);
    ERRDEFER(*trace = *trace * 10 + 9);
    LET a IS(res_int_ok(10));
    DEFER(*trace = *trace * 10 + 2);
    LET b IS(fail_mid ? res_int_err(-1) : res_int_ok(32));
    RETURN(a + b);
  );
}

int main(void) {
  int trace_ok = 0;
  res_int r1 = run_txn(false, &trace_ok);
  CHECK(r1.has_value && r1.value == 42);
  CHECK(trace_ok == 21);

  int trace_err = 0;
  res_int r2 = run_txn(true, &trace_err);
  CHECK(!r2.has_value && r2.error == -1);
  CHECK(trace_err == 291);
  return 0;
}
