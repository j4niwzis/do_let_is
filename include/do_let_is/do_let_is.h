#ifndef DO_LET_IS_DO_LET_IS_H
#define DO_LET_IS_DO_LET_IS_H

#define DO_LET_IS_OUT

#define DO_LET_IS_EVAL_2(...) __VA_ARGS__
#define DO_LET_IS_EVAL_1(...) DO_LET_IS_EVAL_2(DO_LET_IS_EVAL_2(DO_LET_IS_EVAL_2(DO_LET_IS_EVAL_2(__VA_ARGS__))))
#define DO_LET_IS_EVAL_0(...) DO_LET_IS_EVAL_1(DO_LET_IS_EVAL_1(DO_LET_IS_EVAL_1(DO_LET_IS_EVAL_1(__VA_ARGS__))))
#define DO_LET_IS_EVAL(...) DO_LET_IS_EVAL_0(DO_LET_IS_EVAL_0(DO_LET_IS_EVAL_0(DO_LET_IS_EVAL_0(__VA_ARGS__))))

#define DO_LET_IS_DELAY_OPEN_BRACE_0() (
#define DO_LET_IS_DELAY_OPEN_BRACE_1() DO_LET_IS_DELAY_OPEN_BRACE_0 DO_LET_IS_OUT()

#define DO_LET_IS_DELAY_OUT_0() DO_LET_IS_OUT
#define DO_LET_IS_DELAY_OUT_1() DO_LET_IS_DELAY_OUT_0 DO_LET_IS_OUT()
#define DO_LET_IS_DELAY_OUT_2() DO_LET_IS_DELAY_OUT_1 DO_LET_IS_OUT()
#define DO_LET_IS_DELAY_OUT_3() DO_LET_IS_DELAY_OUT_2 DO_LET_IS_OUT()

#define DO_LET_IS_CONCAT_1(a, b, ...) a##b __VA_ARGS__

#define DO_LET_IS_CONCAT_0(...) DO_LET_IS_CONCAT_1 DO_LET_IS_OUT(__VA_ARGS__)

#define DO_LET_IS_INC_0 1
#define DO_LET_IS_INC_1 2
#define DO_LET_IS_INC_2 3
#define DO_LET_IS_INC_3 4
#define DO_LET_IS_INC_4 5
#define DO_LET_IS_INC_5 6
#define DO_LET_IS_INC_6 7
#define DO_LET_IS_INC_7 8
#define DO_LET_IS_INC_8 9
#define DO_LET_IS_INC_9 10
#define DO_LET_IS_INC_10 11
#define DO_LET_IS_INC_11 12
#define DO_LET_IS_INC_12 13
#define DO_LET_IS_INC_13 14
#define DO_LET_IS_INC_14 15
#define DO_LET_IS_INC_15 16
#define DO_LET_IS_INC_16 17
#define DO_LET_IS_INC_17 18
#define DO_LET_IS_INC_18 19
#define DO_LET_IS_INC_19 20
#define DO_LET_IS_INC_20 21
#define DO_LET_IS_INC_21 22
#define DO_LET_IS_INC_22 23
#define DO_LET_IS_INC_23 24
#define DO_LET_IS_INC_24 25
#define DO_LET_IS_INC_25 26
#define DO_LET_IS_INC_26 27
#define DO_LET_IS_INC_27 28
#define DO_LET_IS_INC_28 29
#define DO_LET_IS_INC_29 30
#define DO_LET_IS_INC_30 31
#define DO_LET_IS_INC_31 32
#define DO_LET_IS_INC_32 33
#define DO_LET_IS_INC_33 34
#define DO_LET_IS_INC_34 35
#define DO_LET_IS_INC_35 36
#define DO_LET_IS_INC_36 37
#define DO_LET_IS_INC_37 38
#define DO_LET_IS_INC_38 39
#define DO_LET_IS_INC_39 40
#define DO_LET_IS_INC_40 41
#define DO_LET_IS_INC_41 42

#define DO_LET_IS_DEC_1 0
#define DO_LET_IS_DEC_2 1
#define DO_LET_IS_DEC_3 2
#define DO_LET_IS_DEC_4 3
#define DO_LET_IS_DEC_5 4
#define DO_LET_IS_DEC_6 5
#define DO_LET_IS_DEC_7 6
#define DO_LET_IS_DEC_8 7
#define DO_LET_IS_DEC_9 8
#define DO_LET_IS_DEC_10 9
#define DO_LET_IS_DEC_11 10
#define DO_LET_IS_DEC_12 11
#define DO_LET_IS_DEC_13 12
#define DO_LET_IS_DEC_14 13
#define DO_LET_IS_DEC_15 14
#define DO_LET_IS_DEC_16 15
#define DO_LET_IS_DEC_17 16
#define DO_LET_IS_DEC_18 17
#define DO_LET_IS_DEC_19 18
#define DO_LET_IS_DEC_20 19
#define DO_LET_IS_DEC_21 20
#define DO_LET_IS_DEC_22 21
#define DO_LET_IS_DEC_23 22
#define DO_LET_IS_DEC_24 23
#define DO_LET_IS_DEC_25 24
#define DO_LET_IS_DEC_26 25
#define DO_LET_IS_DEC_27 26
#define DO_LET_IS_DEC_28 27
#define DO_LET_IS_DEC_29 28
#define DO_LET_IS_DEC_30 29
#define DO_LET_IS_DEC_31 30
#define DO_LET_IS_DEC_32 31
#define DO_LET_IS_DEC_33 32
#define DO_LET_IS_DEC_34 33
#define DO_LET_IS_DEC_35 34
#define DO_LET_IS_DEC_36 35
#define DO_LET_IS_DEC_37 36
#define DO_LET_IS_DEC_38 37
#define DO_LET_IS_DEC_39 38
#define DO_LET_IS_DEC_40 39
#define DO_LET_IS_DEC_41 40
#define DO_LET_IS_DEC_42 41
// ...

#define DO_LET_IS_DEC(x) DO_LET_IS_DEC_##x

#define DO_LET_IS_INC(x) DO_LET_IS_INC_##x

#define DO_LET_IS_LAMBDA_CAPTURE &

#define DO_LET_IS_LET ), _LET_IS(

#define DO_LET_IS_IS(...) , (__VA_ARGS__)) \
  , _CODE(

#define DO_LET_IS_WHILE_0(...) , (_CODE(__VA_ARGS__))) \
  , _CODE(

#define DO_LET_IS_WHILE(cond) ), _WHILE((cond) DO_LET_IS_WHILE_0

#define DO_LET_IS_CONTINUE ), _CONTINUE, _CODE(
#define DO_LET_IS_BREAK ), _BREAK, _CODE(

#define DO_LET_IS_IF__1(...) , (_CODE(__VA_ARGS__))) , \
  _CODE(

#define DO_LET_IS_IF__0(...) , (_CODE(__VA_ARGS__))DO_LET_IS_IF__1

#define DO_LET_IS_IF(cond) ) , _IF((cond) DO_LET_IS_IF__0

#define DO_LET_IS_IF_CONSTEXPR(cond) ) , _IF( constexpr(cond) DO_LET_IS_IF__0

#define DO_LET_IS_IF_CONSTEVAL ), _IF( consteval DO_LET_IS_IF__0

#define DO_LET_IS_PARSE_DO_ITERATION_HELPER(...) DO_LET_IS_PARSE_DO_ITERATION DO_LET_IS_OUT(__VA_ARGS__)

#define DO_LET_IS_DO_WORK_LET_IS(name, is, id_while, id_if, ...)                              \
  return ::doletis::bind(is, [DO_LET_IS_LAMBDA_CAPTURE](auto&& name) {                        \
    DO_LET_IS_PARSE_DO_ITERATION_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, id_if, __VA_ARGS__) \
  });

#define DO_LET_IS_CLOSE_MACRO(...) , __VA_ARGS__ )

#define DO_LET_IS_DO_TABLE_LET_IS(name, is) DO_LET_IS_DO_WORK_LET_IS DO_LET_IS_DELAY_OPEN_BRACE_0() name, is DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_DO_WORK_CODE(id_while, id_if, ...) \
  __VA_OPT__(DO_LET_IS_PARSE_DO_ITERATION_HELPER DO_LET_IS_DELAY_OUT_0()(id_while, id_if, __VA_ARGS__))

#define DO_LET_IS_DO_TABLE_CODE(...) __VA_ARGS__ DO_LET_IS_DO_WORK_CODE

#define DO_LET_IS_UNWRAP(...) __VA_ARGS__

#define DO_LET_IS_DO_WORK_WHILE(cond, body, id_while, id_if, ...)                                                      \
  return [DO_LET_IS_LAMBDA_CAPTURE](this const auto& while_self_##id_while, bool break_flag = false) {                 \
    if (break_flag || !cond) {                                                                                         \
      DO_LET_IS_PARSE_DO_ITERATION_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, id_if, __VA_ARGS__)                        \
    }                                                                                                                  \
    DO_LET_IS_PARSE_DO_ITERATION_HELPER DO_LET_IS_DELAY_OUT_3()(DO_LET_IS_INC(id_while), id_if, DO_LET_IS_UNWRAP body, \
                                                                _CODE(return while_self_##id_while();))                \
  }();

#define DO_LET_IS_DO_TABLE_WHILE(cond, body) DO_LET_IS_DO_WORK_WHILE DO_LET_IS_DELAY_OPEN_BRACE_0() cond, body DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_DO_WORK_CONTINUE_BASE(break_flag, id_while, id_if, ...) \
  return DO_LET_IS_CONCAT_0(while_self_, DO_LET_IS_DEC(id_while))(break_flag);

#define DO_LET_IS_DO_TABLE_CONTINUE DO_LET_IS_DO_WORK_CONTINUE_BASE DO_LET_IS_DELAY_OPEN_BRACE_0() false DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_DO_TABLE_BREAK DO_LET_IS_DO_WORK_CONTINUE_BASE DO_LET_IS_DELAY_OPEN_BRACE_0() true DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_DO_WORK_IF(check, body, els, id_while, id_if, ...)                                                     \
  return [DO_LET_IS_LAMBDA_CAPTURE](this const auto& if_self_##id_if, bool is_cont = false) {                            \
    if (is_cont) {                                                                                                       \
      DO_LET_IS_PARSE_DO_ITERATION_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, DO_LET_IS_INC(id_if), __VA_ARGS__)           \
    }                                                                                                                    \
    if check {                                                                                                           \
      DO_LET_IS_PARSE_DO_ITERATION_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, DO_LET_IS_INC(id_if), DO_LET_IS_UNWRAP body, \
                                                                  _CODE(return if_self_##id_if(true);))                  \
    } else {                                                                                                             \
      DO_LET_IS_PARSE_DO_ITERATION_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, DO_LET_IS_INC(id_if), DO_LET_IS_UNWRAP els,  \
                                                                  _CODE(return if_self_##id_if(true);))                  \
    }                                                                                                                    \
  }();

#define DO_LET_IS_DO_TABLE_IF(check, body, els) DO_LET_IS_DO_WORK_IF DO_LET_IS_DELAY_OPEN_BRACE_0() check, body, els DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_PARSE_DO_ITERATION(id_while, id_if, check, ...) DO_LET_IS_DO_TABLE##check(id_while, id_if __VA_OPT__(, ) __VA_ARGS__)

#define DO_LET_IS_DO(...) [&] { DO_LET_IS_EVAL(DO_LET_IS_PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))) }()

#define DO_LET_IS_DO_GLOBAL(...) [] { DO_LET_IS_EVAL(DO_LET_IS_PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))) }()

namespace doletis {}  // namespace doletis

#endif
