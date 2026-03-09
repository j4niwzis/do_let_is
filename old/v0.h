#ifndef DO_LET_IS
#define DO_LET_IS
#define OUT

#define EVAL_2(...) __VA_ARGS__
#define EVAL_1(...) EVAL_2(EVAL_2(EVAL_2(EVAL_2(__VA_ARGS__))))
#define EVAL_0(...) EVAL_1(EVAL_1(EVAL_1(EVAL_1(__VA_ARGS__))))
#define EVAL(...) EVAL_0(EVAL_0(EVAL_0(EVAL_0(__VA_ARGS__))))

#define DELAY_OPEN_BRACE_0() (
#define DELAY_OPEN_BRACE_1() DELAY_OPEN_BRACE_0 OUT()

#define DELAY_OUT_0() OUT
#define DELAY_OUT_1() DELAY_OUT_0 OUT()
#define DELAY_OUT_2() DELAY_OUT_1 OUT()
#define DELAY_OUT_3() DELAY_OUT_2 OUT()

#define EVAL2(...) __VA_ARGS__

#define CONCAT_1(a, b, ...) a##b __VA_ARGS__

#define CONCAT_0(...) CONCAT_1 OUT(__VA_ARGS__)

#define LAMBDA_CAPTURE &

#define LET ), _LET_IS(

#define IS(...) , (__VA_ARGS__)) \
  , _CODE(

#define PARSE_DO_ITERATION_HELPER(...) PARSE_DO_ITERATION OUT(__VA_ARGS__)

#define DO_WORK_LET_IS(name, is, ...) \
  return ::bind(is, [LAMBDA_CAPTURE](auto&& name) { PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(__VA_ARGS__) });

#define CLOSE_MACRO(...) , __VA_ARGS__ )

#define DO_TABLE_LET_IS(name, is) DO_WORK_LET_IS DELAY_OPEN_BRACE_0() name, is CLOSE_MACRO

#define DO_WORK_CODE(...) __VA_OPT__(PARSE_DO_ITERATION_HELPER DELAY_OUT_0()(__VA_ARGS__))

#define DO_TABLE_CODE(...) __VA_ARGS__ DO_WORK_CODE

#define PARSE_DO_ITERATION(check, ...) DO_TABLE##check(__VA_ARGS__)

#define DO(...) [&] { EVAL(PARSE_DO_ITERATION(_CODE(__VA_ARGS__))) }()

#define DO_GLOBAL(...) [] { EVAL(PARSE_DO_ITERATION(_CODE(__VA_ARGS__))) }()

namespace do_let_is {}  // namespace do_let_is
#endif
