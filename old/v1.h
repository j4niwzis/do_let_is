#ifndef DO_LET_IS
#define DO_LET_IS

#define OUT

#define EVAL_2(...) __VA_ARGS__
#define EVAL_1(...) EVAL_2(EVAL_2(EVAL_2(EVAL_2(__VA_ARGS__))))
#define EVAL_0(...) EVAL_1(EVAL_1(EVAL_1(EVAL_1(__VA_ARGS__))))
#define EVAL(...) EVAL_0(EVAL_0(EVAL_0(EVAL_0(__VA_ARGS__))))

#define VOID(...)

#define DELAY_COMMA_0() ,
#define DELAY_COMMA_1() DELAY_COMMA_0 OUT()
#define DELAY_COMMA_2() DELAY_COMMA_1 OUT()
#define DELAY_COMMA_3() DELAY_COMMA_2 OUT()
#define DELAY_COMMA_4() DELAY_COMMA_3 OUT()

#define DELAY_OPEN_BRACE_0() (
#define DELAY_OPEN_BRACE_1() DELAY_OPEN_BRACE_0 OUT()
#define DELAY_OPEN_BRACE_2() DELAY_OPEN_BRACE_1 OUT()
#define DELAY_OPEN_BRACE_3() DELAY_OPEN_BRACE_2 OUT()

#define DELAY_CLOSE_BRACE_0() )
#define DELAY_CLOSE_BRACE_1() DELAY_CLOSE_BRACE_0 OUT()
#define DELAY_CLOSE_BRACE_2() DELAY_CLOSE_BRACE_1 OUT()
#define DELAY_CLOSE_BRACE_3() DELAY_CLOSE_BRACE_2 OUT()

#define DELAY_OUT_0() OUT
#define DELAY_OUT_1() DELAY_OUT_0 OUT()
#define DELAY_OUT_2() DELAY_OUT_1 OUT()
#define DELAY_OUT_3() DELAY_OUT_2 OUT()
#define DELAY_OUT_4() DELAY_OUT_3 OUT()

#define EVAL2(...) __VA_ARGS__

#define CONCAT_1(a, b, ...) a##b __VA_ARGS__

#define CONCAT_0(...) CONCAT_1 OUT(__VA_ARGS__)

#define CONCAT(...) EVAL2(CONCAT_0(__VA_ARGS__))

#define IF_ELSE_1(test, next, ...) next OUT
#define IF_ELSE_0(test, next, ...) IF_ELSE_1(test, next, __VA_ARGS__, 0)

#define IF_ELSE(condition, t, f) IF_ELSE_0(condition OUT t, f)

/*
#define CHECK_IF_ELSE 0,

IF_ELSE(CHECK_IF_ELSE, true, false)
  => IF_ELSE_0(0, OUT true, false)
  => IF_ELSE_1(0, true, false, 0)
  => true OUT

IF_ELSE(FALSE, true, false)
  => IF_ELSE_0(FALSE OUT true, false)
  => IF_ELSE_1(FALSE true, false, 0)
  => false OUT
*/

#define IS_1_1 0,

#define IF_1(v, t, f) IF_ELSE(IS_1_##v, t, f)

#ifndef LAMBDA_CAPTURE
#define LAMBDA_CAPTURE &
#endif

#define LET , _LET_IS(

#define IS(...) , (__VA_ARGS__)) \
  ,

#define PARSE_DO_ITERATION_HELPER(...) PARSE_DO_ITERATION OUT(__VA_ARGS__)

#define DO_CHECK_LET_IS(...) 0,

#define DO_WORK_LET_IS_0(name, is, ...)                                             \
  0, return ::bind(is, ([LAMBDA_CAPTURE](auto&& name) {                             \
                     PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(0, void() __VA_ARGS__) \
                   }));

#define DO_WORK_LET_IS(name, is) DO_WORK_LET_IS_0 DELAY_OPEN_BRACE_0() name, is,

#define PARSE_DO_ITERATION(flag, check, ...)                                                                                \
  EVAL2(IF_ELSE OUT(DO_WORK##check IF_ELSE(DO_CHECK##check, , VOID DELAY_OPEN_BRACE_3()) __VA_ARGS__ DELAY_CLOSE_BRACE_0(), \
                    __VA_OPT__(VOID),                                                                                       \
                    IF_1(flag, DELAY_COMMA_3, VOID)() check __VA_OPT__(PARSE_DO_ITERATION_HELPER OUT)))                     \
  __VA_OPT__((1, __VA_ARGS__))

#define DO(...)                                      \
  [&] {                                              \
    EVAL(PARSE_DO_ITERATION(0, void(); __VA_ARGS__)) \
  }()

#endif
