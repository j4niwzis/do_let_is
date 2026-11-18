#include <do_let_is/async.h>
#include <do_let_is/short.h>
#include <stdlib.h>

#define CHECK(cond) \
  do {              \
    if (!(cond)) {  \
      abort();      \
    }               \
  } while (0)

DOLETIS_DEFINE_TASK(int, task_int)

static task_int async_sum(task_int a, task_int b, int* cleanup_log) {
  return DO(
    DEFER(*cleanup_log = *cleanup_log * 10 + 1);
    ERRDEFER(*cleanup_log = *cleanup_log * 10 + 9);
    LET x IS(a);
    DEFER(*cleanup_log = *cleanup_log * 10 + 2);
    LET y IS(b);
    RETURN(x + y);
  );
}

int main(void) {
  doletis_executor ex;
  doletis_executor_init(&ex);

  task_int_promise p1;
  task_int_promise p2;
  int log1 = 0;
  task_int t1 = async_sum(task_int_pending(&p1), task_int_pending(&p2), &log1);

  task_int_resolve(&ex, &p2, 30);
  task_int_resolve(&ex, &p1, 12);

  int out = 0;
  CHECK(task_int_await(&ex, t1, &out));
  CHECK(out == 42);
  CHECK(log1 == 21);

  task_int_promise p3;
  task_int_promise p4;
  int log2 = 0;
  task_int t2 = async_sum(task_int_pending(&p3), task_int_pending(&p4), &log2);
  task_int_resolve(&ex, &p3, 10);
  doletis_executor_run(&ex);
  task_int_cancel(&t2);
  CHECK(log2 == 291);
  task_int_promise_drop(&p4);
  return 0;
}
