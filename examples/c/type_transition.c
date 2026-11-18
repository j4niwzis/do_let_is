#include <do_let_is/all_short.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(cond) \
  do {              \
    if (!(cond)) {  \
      abort();      \
    }               \
  } while (0)

typedef struct {
  double total;
  const char* label;
} summary_t;

DOLETIS_DEFINE_OPTIONAL(int, opt_int)
DOLETIS_DEFINE_OPTIONAL(double, opt_double)
DOLETIS_DEFINE_OPTIONAL(summary_t, opt_summary)
DOLETIS_DEFINE_OPTIONAL_CONV(opt_int, int, opt_summary)
DOLETIS_DEFINE_OPTIONAL_CONV(opt_double, double, opt_summary)

DOLETIS_DEFINE_TASK(int, task_int)
DOLETIS_DEFINE_TASK(double, task_double)
DOLETIS_DEFINE_TASK(summary_t, task_summary)
DOLETIS_DEFINE_TASK_CONV(task_int, int, task_summary, summary_t)
DOLETIS_DEFINE_TASK_CONV(task_double, double, task_summary, summary_t)

static opt_summary build_summary(opt_int count, opt_double factor) {
  return DO(
    LET n IS(count);
    LET k IS(factor);
    RETURN((summary_t){.total = (double)n * k, .label = "ok"});
  );
}

int main(void) {
  opt_summary s1 = build_summary(opt_int_some(8), opt_double_some(1.5));
  CHECK(s1.has_value && s1.value.total == 12.0 && strcmp(s1.value.label, "ok") == 0);

  opt_summary s2 = build_summary(opt_int_none(), opt_double_some(1.5));
  CHECK(!s2.has_value);

  doletis_executor ex;
  doletis_executor_init(&ex);
  task_int_promise p_n;
  task_double_promise p_k;
  int trace = 0;
  int* trace_ptr = &trace;
  task_int tn = task_int_pending(&p_n);
  task_double tk = task_double_pending(&p_k);

  task_summary job = DO(
    DEFER(*trace_ptr += 1);
    LET n IS(tn);
    LET k IS(tk);
    RETURN((summary_t){.total = (double)n * k, .label = "async"});
  );

  task_int_resolve(&ex, &p_n, 20);
  task_double_resolve(&ex, &p_k, 2.1);

  summary_t out = {};
  CHECK(task_summary_await(&ex, job, &out));
  CHECK(out.total == 42.0 && strcmp(out.label, "async") == 0);
  CHECK(trace == 1);
  return 0;
}
