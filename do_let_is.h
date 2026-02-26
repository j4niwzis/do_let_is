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

#define INC_0 1
#define INC_1 2
#define INC_2 3

#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
// ...

#define DEC(x) DEC_##x

#define INC(x) INC_##x

#define IS_1_1 0,

#define IF_1(v, t, f) IF_ELSE(IS_1_##v, t, f)

#define EVAL3(...) __VA_ARGS__

#ifndef LAMBDA_CAPTURE
#define LAMBDA_CAPTURE &
#endif

#define LET , _LET_IS(

#define IS(...) , (__VA_ARGS__)) \
  ,

#define WHILE_0(...) , (__VA_ARGS__)) \
  ,

#define WHILE(cond) , _WHILE((cond) WHILE_0

#define CONTINUE , _CONTINUE,
#define BREAK , _BREAK,

#define IF__1(...) , (__VA_ARGS__)) \
  ,

#define IF__0(...) , (__VA_ARGS__)IF__1

#define IF(cond) , _IF((cond) IF__0

#define IF_CONSTEXPR_1(...) , (__VA_ARGS__)) \
  ,

#define IF_CONSTEXPR_0(...) , (__VA_ARGS__)IF_CONSTEXPR_1

#define IF_CONSTEXPR(cond) , _IF_CONSTEXPR((cond) IF_CONSTEXPR_0

#define IF_CONSTEVAL_0(...) , (__VA_ARGS__)) \
  ,

#define IF_CONSTEVAL(...) , _IF_CONSTEVAL((__VA_ARGS__) IF_CONSTEVAL_0

#define PARSE_DO_ITERATION_HELPER(...) PARSE_DO_ITERATION OUT(__VA_ARGS__)

#define DO_CHECK_LET_IS(...) 0,

#define DO_WORK_LET_IS_0(name, is, id_while, id_if, ...)                                             \
  0, return ::bind(is, ([LAMBDA_CAPTURE](auto&& name) mutable {                                              \
                     PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(0, id_while, id_if, void() __VA_ARGS__) \
                   }));

#define DO_WORK_LET_IS(name, is) DO_WORK_LET_IS_0 DELAY_OPEN_BRACE_0() name, is,

#define DO_CHECK_WHILE(...) 0,

#define DO_WORK_WHILE_0(cond, body, id_while, id_if, ...)                                                                \
  0, return ([LAMBDA_CAPTURE](this auto&& while_self_##id_while, bool break_flag = false) {                              \
    if(break_flag || !cond) {                                                                                            \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(0, INC(id_while), id_if, void(); __VA_ARGS__)                              \
    }                                                                                                                    \
    PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(0, INC(id_while), id_if, void(); EVAL3 body return while_self_##id_while();) \
  }());

#define DO_WORK_WHILE(cond, body) DO_WORK_WHILE_0 DELAY_OPEN_BRACE_0() cond, body,

#define DO_CHECK_CONTINUE 0,

#define DO_WORK_CONTINUE_0(id_while, id_if, ...)   \
  0, return ([LAMBDA_CAPTURE] {                    \
    return CONCAT_0(while_self_, DEC(id_while))(); \
  }());

#define DO_WORK_CONTINUE DO_WORK_CONTINUE_0 DELAY_OPEN_BRACE_0()

#define DO_CHECK_BREAK 0,

#define DO_WORK_BREAK_0(id_while, id_if, ...)          \
  0, return ([LAMBDA_CAPTURE] {                        \
    return CONCAT_0(while_self_, DEC(id_while))(true); \
  }());

#define DO_WORK_BREAK DO_WORK_BREAK_0 DELAY_OPEN_BRACE_0()

#define DO_WORK_IF_BASE(check, body, els, id_while, id_if, ...)                                                      \
  0, return ([LAMBDA_CAPTURE] {                                                                                      \
    auto if_cont_##id_if = [LAMBDA_CAPTURE] {                                                                        \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(0, id_while, INC(id_if), void(); __VA_ARGS__)                          \
    };                                                                                                               \
    if check {                                                                                                       \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(0, id_while, INC(id_if), void(); EVAL3 body return if_cont_##id_if();) \
    } else {                                                                                                         \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(0, id_while, INC(id_if), void(); EVAL3 els return if_cont_##id_if();)  \
    }                                                                                                                \
  }());

#define DO_CHECK_IF(...) 0,

#define DO_WORK_IF(cond, body, els) DO_WORK_IF_BASE DELAY_OPEN_BRACE_0()(cond), body, els,

#define DO_CHECK_IF_CONSTEXPR(...) 0,

#define DO_WORK_IF_CONSTEXPR(cond, body, els) DO_WORK_IF_BASE DELAY_OPEN_BRACE_0() constexpr(cond), body, els,

#define DO_CHECK_IF_CONSTEVAL(...) 0,

#define DO_WORK_IF_CONSTEVAL(body, els) DO_WORK_IF_BASE DELAY_OPEN_BRACE_0() consteval, body, els,

#define PARSE_DO_ITERATION(flag, id_while, id_if, check, ...)                                           \
  EVAL2(IF_ELSE OUT(DO_WORK##check IF_ELSE(DO_CHECK##check, , VOID DELAY_OPEN_BRACE_3()) id_while,      \
                    id_if,                                                                              \
                    __VA_ARGS__ DELAY_CLOSE_BRACE_0(),                                                  \
                    __VA_OPT__(VOID),                                                                   \
                    IF_1(flag, DELAY_COMMA_3, VOID)() check __VA_OPT__(PARSE_DO_ITERATION_HELPER OUT))) \
  __VA_OPT__((1, id_while, id_if, __VA_ARGS__))

#define DO(...)                                            \
  [&] {                                                    \
    EVAL(PARSE_DO_ITERATION(0, 0, 0, void(); __VA_ARGS__)) \
  }()

#define DO_GLOBAL(...)                                     \
  [] {                                                     \
    EVAL(PARSE_DO_ITERATION(0, 0, 0, void(); __VA_ARGS__)) \
  }()
#endif
