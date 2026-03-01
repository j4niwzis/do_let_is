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

#define INC_0 1
#define INC_1 2
#define INC_2 3

#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
// ...

#define DEC(x) DEC_##x

#define INC(x) INC_##x

#define LAMBDA_CAPTURE &

#define LET ), _LET_IS(

#define IS(...) , (__VA_ARGS__)) \
  , _CODE(

#define WHILE_0(...) , (_CODE(__VA_ARGS__))) \
  , _CODE(

#define WHILE(cond) ), _WHILE((cond) WHILE_0

#define CONTINUE ), _CONTINUE, _CODE(
#define BREAK ), _BREAK, _CODE(

#define IF__1(...) , (_CODE(__VA_ARGS__))) , \
  _CODE(

#define IF__0(...) , (_CODE(__VA_ARGS__))IF__1

#define IF(cond) ) , _IF((cond) IF__0

#define IF_CONSTEXPR(cond) ) , _IF( constexpr(cond) IF__0

#define IF_CONSTEVAL ), _IF( consteval IF__0

#define PARSE_DO_ITERATION_HELPER(...) PARSE_DO_ITERATION OUT(__VA_ARGS__)

#define DO_WORK_LET_IS(name, is, id_while, id_if, ...)                    \
  return ::bind(is, [LAMBDA_CAPTURE](auto&& name) {                       \
    PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(id_while, id_if, __VA_ARGS__) \
  });

#define CLOSE_MACRO(...) , __VA_ARGS__ )

#define DO_TABLE_LET_IS(name, is) DO_WORK_LET_IS DELAY_OPEN_BRACE_0() name, is CLOSE_MACRO

#define DO_WORK_CODE(id_while, id_if, ...) __VA_OPT__(PARSE_DO_ITERATION_HELPER DELAY_OUT_0()(id_while, id_if, __VA_ARGS__))

#define DO_TABLE_CODE(...) __VA_ARGS__ DO_WORK_CODE

#define UNWRAP(...) __VA_ARGS__

#define DO_WORK_WHILE(cond, body, id_while, id_if, ...)                                                                \
  return [LAMBDA_CAPTURE](this const auto& while_self_##id_while, bool break_flag = false) {                           \
    if(break_flag || !cond) {                                                                                          \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(id_while, id_if, __VA_ARGS__)                                            \
    }                                                                                                                  \
    PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(INC(id_while), id_if, UNWRAP body, _CODE(return while_self_##id_while();)) \
  }();

#define DO_TABLE_WHILE(cond, body) DO_WORK_WHILE DELAY_OPEN_BRACE_0() cond, body CLOSE_MACRO

#define DO_WORK_CONTINUE_BASE(break_flag, id_while, id_if, ...) return CONCAT_0(while_self_, DEC(id_while))(break_flag);

#define DO_TABLE_CONTINUE DO_WORK_CONTINUE_BASE DELAY_OPEN_BRACE_0() false CLOSE_MACRO

#define DO_TABLE_BREAK DO_WORK_CONTINUE_BASE DELAY_OPEN_BRACE_0() true CLOSE_MACRO

#define DO_WORK_IF(check, body, els, id_while, id_if, ...)                                                             \
  return [LAMBDA_CAPTURE](this const auto& if_self_##id_if, bool is_cont = false) {                                    \
    if(is_cont) {                                                                                                      \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(id_while, INC(id_if), __VA_ARGS__)                                       \
    }                                                                                                                  \
    if check {                                                                                                         \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(id_while, INC(id_if), UNWRAP body, _CODE(return if_self_##id_if(true);)) \
    } else {                                                                                                           \
      PARSE_DO_ITERATION_HELPER DELAY_OUT_3()(id_while, INC(id_if), UNWRAP els, _CODE(return if_self_##id_if(true);))  \
    }                                                                                                                  \
  }();

#define DO_TABLE_IF(check, body, els) DO_WORK_IF DELAY_OPEN_BRACE_0() check, body, els CLOSE_MACRO

#define PARSE_DO_ITERATION(id_while, id_if, check, ...) DO_TABLE##check(id_while, id_if __VA_OPT__(, ) __VA_ARGS__)

#define DO(...)                                        \
  [&] {                                                \
    EVAL(PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))) \
  }()

#define DO_GLOBAL(...)                                 \
  [] {                                                 \
    EVAL(PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))) \
  }()

#endif
