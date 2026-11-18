#include <do_let_is/generator.h>
#include <do_let_is/short.h>
#include <stdlib.h>

#define CHECK(cond) \
  do {              \
    if (!(cond)) {  \
      abort();      \
    }               \
  } while (0)

DOLETIS_DEFINE_GENERATOR(int, gen_int)

static gen_int naturals_with_cleanup(int* closed_count) {
  return GENERATOR(gen_int,
    DEFER(*closed_count += 1);
    __block int i = 0;
    YIELD(42);
    WHILE(true)(
      YIELD(i);
      ++i;
    )
    RETURN(gen_int_end());
  );
}

int main(void) {
  int closed = 0;
  gen_int g = naturals_with_cleanup(&closed);
  int v = 0;
  CHECK(gen_int_next(&g, &v) && v == 42);
  CHECK(gen_int_next(&g, &v) && v == 0);
  CHECK(gen_int_next(&g, &v) && v == 1);
  CHECK(closed == 0);
  gen_int_close(&g);
  CHECK(closed == 1);
  return 0;
}
