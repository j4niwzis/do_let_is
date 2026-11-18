#include "sample_c_api1.h"

#ifdef DO
#error "DO must be undefined after sample_c_api1.h"
#endif

#include <do_let_is/all_short.h>
#include "sample_c_api2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(cond)                                                              \
  do {                                                                           \
    if (!(cond)) {                                                               \
      fprintf(stderr, "CHECK failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      abort();                                                                   \
    }                                                                            \
  } while (0)

typedef struct {
  double val;
  const char* tag;
} pair_out_t;

DOLETIS_DEFINE_OPTIONAL(int, opt_int)
DOLETIS_DEFINE_OPTIONAL(double, opt_double)
DOLETIS_DEFINE_OPTIONAL_CONV(opt_int, int, opt_double)
DOLETIS_DEFINE_EXPECTED(int, int, res_int)
DOLETIS_DEFINE_EXPECTED(pair_out_t, int, res_pair)
DOLETIS_DEFINE_EXPECTED_CONV(res_int, int, res_pair)
DOLETIS_DEFINE_VEC(int, 32, vec_int)
DOLETIS_DEFINE_VEC(double, 32, vec_double)
DOLETIS_DEFINE_VEC_CONV(vec_int, int, vec_double, 32)
DOLETIS_DEFINE_GENERATOR(int, gen_int)
DOLETIS_DEFINE_TASK(int, task_int)
DOLETIS_DEFINE_TASK(double, task_double)
DOLETIS_DEFINE_TASK_CONV(task_int, int, task_double, double)

static void test_optional_and_control_flow(void) {
  opt_int r1 = DO(
    LET a IS(opt_int_some(19));
    LET b IS(opt_int_some(23));
    RETURN(a + b);
  );
  CHECK(r1.has_value && r1.value == 42);

  opt_int r2 = DO(
    LET a IS(opt_int_some(19));
    LET b IS(opt_int_none());
    RETURN(a + b);
  );
  CHECK(!r2.has_value);

  opt_int r3 = DO(
    __block int i = 0;
    __block int acc = 0;
    WHILE(i < 10)(
      IF(i == 6)(
        BREAK;
      )(
        IF(i % 2 == 1)(
          ++i;
          CONTINUE;
        )(
          LET v IS(opt_int_some(i));
          acc += v;
          ++i;
        )
      )
    )
    RETURN(acc);
  );
  CHECK(r3.has_value && r3.value == (0 + 2 + 4));
}

static void test_vec_monad(void) {
  vec_int a = {.size = 3, .data = {1, 2, 3}};
  vec_int b = {.size = 2, .data = {10, 20}};
  vec_int out = DO(
    LET x IS(a);
    LET y IS(b);
    RETURN(x + y);
  );
  const int expected[6] = {11, 21, 12, 22, 13, 23};
  CHECK(out.size == 6);
  for (size_t i = 0; i < 6; ++i) {
    CHECK(out.data[i] == expected[i]);
  }
}

static res_int txn_steps(int fail_at, int* log) {
  return DO(
    DEFER(*log = *log * 10 + 1);
    ERRDEFER(*log = *log * 10 + 8);
    LET a IS(fail_at == 1 ? res_int_err(10) : res_int_ok(5));
    DEFER(*log = *log * 10 + 2);
    __block int i = 0;
    __block int sum = a;
    WHILE(i < 3)(
      DEFER(*log = *log * 10 + 3);
      ERRDEFER(*log = *log * 10 + 9);
      IF(fail_at == 2 && i == 1)(
        RETURN(res_int_err(20));
      )(
        IF(i == 2)(
          BREAK;
        )(
          sum += 10;
          ++i;
        )
      )
    )
    RETURN(sum);
  );
}

static void test_defer_and_errdefer(void) {
  int log_ok = 0;
  res_int ok = txn_steps(0, &log_ok);
  CHECK(ok.has_value && ok.value == 25);
  CHECK(log_ok == 33321);

  int log_fail1 = 0;
  res_int f1 = txn_steps(1, &log_fail1);
  CHECK(!f1.has_value && f1.error == 10);
  CHECK(log_fail1 == 81);

  int log_fail2 = 0;
  res_int f2 = txn_steps(2, &log_fail2);
  CHECK(!f2.has_value && f2.error == 20);
  CHECK(log_fail2 == 393281);
}

static gen_int make_range_gen(int limit, int* cleanup_hits) {
  return GENERATOR(gen_int,
    DEFER(*cleanup_hits += 1);
    __block int i = 0;
    YIELD(100);
    WHILE(i < limit)(
      YIELD(i);
      ++i;
    )
    RETURN(gen_int_end());
  );
}

static void test_generator(void) {
  int clean_full = 0;
  gen_int g1 = make_range_gen(3, &clean_full);
  const int exp1[4] = {100, 0, 1, 2};
  int val = 0;
  for (int i = 0; i < 4; ++i) {
    CHECK(gen_int_next(&g1, &val) && val == exp1[i]);
  }
  CHECK(!gen_int_next(&g1, &val));
  CHECK(clean_full == 1);
  gen_int_close(&g1);
  CHECK(clean_full == 1);

  int clean_early = 0;
  gen_int g2 = make_range_gen(1000, &clean_early);
  CHECK(gen_int_next(&g2, &val) && val == 100);
  CHECK(gen_int_next(&g2, &val) && val == 0);
  CHECK(clean_early == 0);
  gen_int_close(&g2);
  CHECK(clean_early == 1);
}

static void test_async_and_trampoline(void) {
  doletis_executor ex;
  doletis_executor_init(&ex);

  task_int heavy = DO(
    __block int i = 0;
    __block int sum = 0;
    WHILE(i < 100000)(
      LET x IS(task_int_ready(1));
      sum += x;
      ++i;
    )
    RETURN(sum);
  );
  int heavy_out = 0;
  CHECK(task_int_await(&ex, heavy, &heavy_out));
  CHECK(heavy_out == 100000);

  task_int_promise p1;
  task_int_promise p2;
  int async_log = 0;
  int* log_ptr = &async_log;
  task_int t1 = task_int_pending(&p1);
  task_int t2 = task_int_pending(&p2);
  task_int pipeline = DO(
    DEFER(*log_ptr = *log_ptr * 10 + 1);
    ERRDEFER(*log_ptr = *log_ptr * 10 + 9);
    LET a IS(t1);
    DEFER(*log_ptr = *log_ptr * 10 + 2);
    LET b IS(t2);
    RETURN(a * b);
  );
  task_int_resolve(&ex, &p2, 7);
  task_int_resolve(&ex, &p1, 6);
  int prod = 0;
  CHECK(task_int_await(&ex, pipeline, &prod));
  CHECK(prod == 42);
  CHECK(async_log == 21);

  task_int_promise p3;
  task_int_promise p4;
  int cancel_log = 0;
  int* clog_ptr = &cancel_log;
  task_int c1 = task_int_pending(&p3);
  task_int c2 = task_int_pending(&p4);
  task_int cancellable = DO(
    DEFER(*clog_ptr = *clog_ptr * 10 + 1);
    ERRDEFER(*clog_ptr = *clog_ptr * 10 + 8);
    LET a IS(c1);
    (void)a;
    DEFER(*clog_ptr = *clog_ptr * 10 + 2);
    ERRDEFER(*clog_ptr = *clog_ptr * 10 + 9);
    LET b IS(c2);
    RETURN(b);
  );
  task_int_resolve(&ex, &p3, 10);
  doletis_executor_run(&ex);
  CHECK(cancel_log == 0);
  task_int_cancel(&cancellable);
  CHECK(cancel_log == 9281);
  task_int_promise_drop(&p4);
}

static void test_type_transitions(void) {
  opt_double od = DO(
    LET x IS(opt_int_some(7));
    LET y IS(opt_double_some(0.5));
    RETURN((double)x + y);
  );
  CHECK(od.has_value && od.value == 7.5);

  opt_double od_none = DO(
    LET x IS(opt_int_none());
    LET y IS(opt_double_some(0.5));
    RETURN((double)x + y);
  );
  CHECK(!od_none.has_value);

  int trace = 0;
  int* trace_ptr = &trace;
  res_pair rp = DO(
    DEFER(*trace_ptr = *trace_ptr * 10 + 1);
    ERRDEFER(*trace_ptr = *trace_ptr * 10 + 9);
    LET count IS(res_int_ok(12));
    RETURN((pair_out_t){.val = (double)count * 3.5, .tag = "converted"});
  );
  CHECK(rp.has_value && rp.value.val == 42.0 && strcmp(rp.value.tag, "converted") == 0);
  CHECK(trace == 1);

  vec_int vi = {.size = 2, .data = {2, 4}};
  vec_double vd = DO(
    LET x IS(vi);
    RETURN((vec_double){.size = 2, .data = {(double)x * 0.5, (double)x * 1.5}});
  );
  CHECK(vd.size == 4 && vd.data[0] == 1.0 && vd.data[1] == 3.0 && vd.data[2] == 2.0 && vd.data[3] == 6.0);

  doletis_executor ex;
  doletis_executor_init(&ex);
  task_int_promise p_int;
  task_double_promise p_dbl;
  int c_log = 0;
  int* clog_ptr = &c_log;
  task_int ti = task_int_pending(&p_int);
  task_double td = task_double_pending(&p_dbl);
  task_double t_conv = DO(
    DEFER(*clog_ptr += 1);
    LET n IS(ti);
    LET f IS(td);
    RETURN((double)n * f);
  );
  task_int_resolve(&ex, &p_int, 16);
  task_double_resolve(&ex, &p_dbl, 2.5);
  double out_d = 0.0;
  CHECK(task_double_await(&ex, t_conv, &out_d));
  CHECK(out_d == 40.0 && c_log == 1);
}

int main(void) {
  c_api1_opt h1 = c_api1_add(c_api1_opt_some(20), c_api1_opt_some(22));
  CHECK(h1.has_value && h1.value == 42);
  int h2_trace = 0;
  c_api2_res h2 = c_api2_txn(false, &h2_trace);
  CHECK(h2.has_value && h2.value == 42 && h2_trace == 1);

  test_optional_and_control_flow();
  test_vec_monad();
  test_defer_and_errdefer();
  test_generator();
  test_async_and_trampoline();
  test_type_transitions();
  return 0;
}
