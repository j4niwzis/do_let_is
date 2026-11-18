#include <do_let_is/c/monads.h>
#include <do_let_is/short.h>
#include <stdlib.h>

#define CHECK(cond) \
  do {              \
    if (!(cond)) {  \
      abort();      \
    }               \
  } while (0)

DOLETIS_DEFINE_OPTIONAL(int, opt_int)

static opt_int add_opt(opt_int a, opt_int b) {
  return DO(
    LET x IS(a);
    LET y IS(b);
    RETURN(x + y);
  );
}

static opt_int sum_slice(const opt_int* xs, size_t n) {
  return DO(
    __block size_t i = 0;
    __block int sum = 0;
    WHILE(i < n)(
      LET v IS(xs[i]);
      sum += v;
      ++i;
    )
    RETURN(sum);
  );
}

int main(void) {
  opt_int ok = add_opt(opt_int_some(20), opt_int_some(22));
  CHECK(ok.has_value && ok.value == 42);

  opt_int bad = add_opt(opt_int_some(20), opt_int_none());
  CHECK(!bad.has_value);

  const opt_int items[3] = {opt_int_some(1), opt_int_some(2), opt_int_some(3)};
  opt_int total = sum_slice(items, 3);
  CHECK(total.has_value && total.value == 6);
  return 0;
}
