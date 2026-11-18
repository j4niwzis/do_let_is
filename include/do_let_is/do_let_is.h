#ifndef __cplusplus
#include "do_let_is/c/do_let_is.h"
#else

#ifndef DO_LET_IS_CXX_DO_LET_IS_H
#define DO_LET_IS_CXX_DO_LET_IS_H

#include <type_traits>
#include <utility>

#include "do_let_is/common.h"

#ifndef DO_LET_IS_LAMBDA_CAPTURE
#define DO_LET_IS_LAMBDA_CAPTURE &
#endif

#define DO_LET_IS_CAP_0 DO_LET_IS_LAMBDA_CAPTURE
#define DO_LET_IS_CAP_1 /* NOLINT */ =, this
#define DO_LET_IS_CAP(c) DO_LET_IS_CAT2(DO_LET_IS_CAP_, c)

#define DO_LET_IS_CXX_PARSE_HELPER(...) DO_LET_IS_CXX_PARSE_DO_ITERATION DO_LET_IS_OUT(__VA_ARGS__)

#define DO_LET_IS_CXX_SCAN_MONAD_0(...)
#define DO_LET_IS_CXX_SCAN_MONAD_1(...)                                                  \
  using _do_monad_t [[maybe_unused]] =                                                   \
      typename ::doletis::internal::unwrap_monad_probe<decltype([&]() {                  \
        DO_LET_IS_SCAN_EVAL(DO_LET_IS_SCAN_STEP(CXX, __VA_ARGS__, _END_SCAN))            \
      }())>::type;

#define DO_LET_IS_CXX_WORK_LET_IS_0(name, is, id_while, id_if, cap, has_monad, ...)                      \
  return [&](auto&& _do_m) -> decltype(auto) {                                                           \
    using _do_cur_m_t = std::remove_cvref_t<decltype(_do_m)>;                                            \
    return ::doletis::internal::invoke_bind_0(                                                           \
        std::forward<decltype(_do_m)>(_do_m),                                                            \
        [DO_LET_IS_CAP(cap)]([[maybe_unused]] auto&& name) {                                             \
          using _do_monad_t [[maybe_unused]] = _do_cur_m_t;                                              \
          DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(id_while, id_if, 0, cap, 1, __VA_ARGS__)    \
        });                                                                                              \
  }(is);

#define DO_LET_IS_CXX_WORK_LET_IS_1(name, is, id_while, id_if, cap, has_monad, ...)                      \
  return [&](auto&& _do_m) -> decltype(auto) {                                                           \
    using _do_cur_m_t = std::remove_cvref_t<decltype(_do_m)>;                                            \
    return ::doletis::internal::invoke_bind(                                                             \
        std::forward<decltype(_do_m)>(_do_m), _do_defer,                                                 \
        [DO_LET_IS_CAP(cap)]([[maybe_unused]] auto&& name) {                                             \
          using _do_monad_t [[maybe_unused]] = _do_cur_m_t;                                              \
          DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(id_while, id_if, 1, cap, 1, __VA_ARGS__)    \
        });                                                                                              \
  }(is);

#define DO_LET_IS_CXX_WORK_LET_IS(name, is, id_while, id_if, has_defer, cap, has_monad, ...) \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_LET_IS_, has_defer)(name, is, id_while, id_if, cap, has_monad, __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_LET_IS(name, is) \
  DO_LET_IS_CXX_WORK_LET_IS DO_LET_IS_DELAY_OPEN_BRACE_0() name, is DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_WORK_DEFER_0(only_err, cleanup, id_while, id_if, cap, has_monad, ...)              \
  {                                                                                                      \
    auto _do_step = [DO_LET_IS_CAP(cap)]() { DO_LET_IS_UNWRAP cleanup; };                                \
    [[maybe_unused]] auto _do_defer =                                                                    \
        ::doletis::internal::make_defer(only_err, _do_step, ::doletis::internal::noop_defer{});          \
    [[maybe_unused]] auto _do_loop_defer =                                                               \
        ::doletis::internal::make_defer(only_err, _do_step, ::doletis::internal::noop_defer{});          \
    [[maybe_unused]] auto _do_if_defer =                                                                 \
        ::doletis::internal::make_defer(only_err, _do_step, ::doletis::internal::noop_defer{});          \
    DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(id_while, id_if, 1, cap, has_monad, __VA_ARGS__)  \
  }

#define DO_LET_IS_CXX_WORK_DEFER_1(only_err, cleanup, id_while, id_if, cap, has_monad, ...)              \
  {                                                                                                      \
    auto _do_prev_all = _do_defer;                                                                       \
    auto _do_prev_loop = _do_loop_defer;                                                                 \
    auto _do_prev_if = _do_if_defer;                                                                     \
    auto _do_step = [DO_LET_IS_CAP(cap)]() { DO_LET_IS_UNWRAP cleanup; };                                \
    {                                                                                                    \
      [[maybe_unused]] auto _do_defer =                                                                  \
          ::doletis::internal::make_defer(only_err, _do_step, _do_prev_all);                             \
      [[maybe_unused]] auto _do_loop_defer =                                                             \
          ::doletis::internal::make_defer(only_err, _do_step, _do_prev_loop);                            \
      [[maybe_unused]] auto _do_if_defer =                                                               \
          ::doletis::internal::make_defer(only_err, _do_step, _do_prev_if);                              \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(id_while, id_if, 1, cap, has_monad, __VA_ARGS__)\
    }                                                                                                    \
  }

#define DO_LET_IS_CXX_WORK_DEFER(cleanup, id_while, id_if, has_defer, cap, has_monad, ...) \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_DEFER_, has_defer)(false, cleanup, id_while, id_if, cap, has_monad, __VA_ARGS__)

#define DO_LET_IS_CXX_WORK_ERRDEFER(cleanup, id_while, id_if, has_defer, cap, has_monad, ...) \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_DEFER_, has_defer)(true, cleanup, id_while, id_if, cap, has_monad, __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_DEFER(cleanup) \
  DO_LET_IS_CXX_WORK_DEFER DO_LET_IS_DELAY_OPEN_BRACE_0() cleanup DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_TABLE_ERRDEFER(cleanup) \
  DO_LET_IS_CXX_WORK_ERRDEFER DO_LET_IS_DELAY_OPEN_BRACE_0() cleanup DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_WORK_RETURN_0(expr, id_while, id_if, cap, has_monad, ...) \
  { return ::doletis::internal::invoke_pure<_do_monad_t>((DO_LET_IS_UNWRAP expr)); }

#define DO_LET_IS_CXX_WORK_RETURN_1(expr, id_while, id_if, cap, has_monad, ...)                          \
  {                                                                                                      \
    auto _do_ret = ::doletis::internal::invoke_pure<_do_monad_t>((DO_LET_IS_UNWRAP expr));              \
    _do_defer(::doletis::internal::is_err_value(_do_ret));                                               \
    return _do_ret;                                                                                      \
  }

#define DO_LET_IS_CXX_WORK_RETURN(expr, id_while, id_if, has_defer, cap, has_monad, ...) \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_RETURN_, has_defer)(expr, id_while, id_if, cap, has_monad __VA_OPT__(, ) __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_RETURN(expr) \
  DO_LET_IS_CXX_WORK_RETURN DO_LET_IS_DELAY_OPEN_BRACE_0() expr DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_WORK_CODE(id_while, id_if, has_defer, cap, has_monad, ...)                         \
  __VA_OPT__(DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_0()(                                         \
      id_while, id_if, has_defer, cap, has_monad, __VA_ARGS__))

#define DO_LET_IS_CXX_TABLE_CODE(...) __VA_ARGS__ DO_LET_IS_CXX_WORK_CODE

#define DO_LET_IS_CXX_WORK_END_WHILE_0(loop_id, id_while, id_if, cap, has_monad, ...) \
  return while_self_##loop_id();

#define DO_LET_IS_CXX_WORK_END_WHILE_1(loop_id, id_while, id_if, cap, has_monad, ...)                    \
  _do_loop_defer(false);                                                                                 \
  return while_self_##loop_id();

#define DO_LET_IS_CXX_WORK_END_WHILE(loop_id, id_while, id_if, has_defer, cap, has_monad, ...)           \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_END_WHILE_, has_defer)(                                              \
      loop_id, id_while, id_if, cap, has_monad __VA_OPT__(, ) __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_END_WHILE(loop_id) \
  DO_LET_IS_CXX_WORK_END_WHILE DO_LET_IS_DELAY_OPEN_BRACE_0() loop_id DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_WORK_WHILE_0(cond, body, id_while, id_if, cap, has_monad, ...)                     \
  return [DO_LET_IS_CAP(cap)](this const auto& while_self_##id_while, bool break_flag = false) {         \
    (void)while_self_##id_while;                                                                         \
    if (break_flag || !cond) {                                                                           \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(id_while, id_if, 0, cap, has_monad, __VA_ARGS__)\
    }                                                                                                    \
    DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                  \
        DO_LET_IS_INC(id_while), id_if, 0, cap, has_monad, DO_LET_IS_UNWRAP body, _END_WHILE(id_while))  \
  }();

#define DO_LET_IS_CXX_WORK_WHILE_1(cond, body, id_while, id_if, cap, has_monad, ...)                     \
  return [DO_LET_IS_CAP(cap)](this const auto& while_self_##id_while, bool break_flag = false) {         \
    (void)while_self_##id_while;                                                                         \
    if (break_flag || !cond) {                                                                           \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(id_while, id_if, 1, cap, has_monad, __VA_ARGS__)\
    }                                                                                                    \
    {                                                                                                    \
      [[maybe_unused]] ::doletis::internal::noop_defer _do_loop_defer{};                                 \
      [[maybe_unused]] ::doletis::internal::noop_defer _do_if_defer{};                                   \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                \
          DO_LET_IS_INC(id_while), id_if, 1, cap, has_monad, DO_LET_IS_UNWRAP body, _END_WHILE(id_while))\
    }                                                                                                    \
  }();

#define DO_LET_IS_CXX_WORK_WHILE_M0_H(cond, body, id_while, id_if, has_defer, cap, has_let, ...)         \
  {                                                                                                      \
    DO_LET_IS_CAT2(DO_LET_IS_CXX_SCAN_MONAD_, has_let)(_WHILE(cond, body), __VA_ARGS__)                  \
    DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_WHILE_, has_defer)(                                                \
        cond, body, id_while, id_if, cap, has_let, __VA_ARGS__)                                          \
  }

#define DO_LET_IS_CXX_WORK_WHILE_M_0(cond, body, id_while, id_if, has_defer, cap, ...)                   \
  DO_LET_IS_CXX_WORK_WHILE_M0_H(cond, body, id_while, id_if, has_defer, cap,                             \
                                DO_LET_IS_HAS_LET(_WHILE(cond, body), __VA_ARGS__), __VA_ARGS__)

#define DO_LET_IS_CXX_WORK_WHILE_M_1(cond, body, id_while, id_if, has_defer, cap, ...)                   \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_WHILE_, has_defer)(cond, body, id_while, id_if, cap, 1, __VA_ARGS__)

#define DO_LET_IS_CXX_WORK_WHILE(cond, body, id_while, id_if, has_defer, cap, has_monad, ...)            \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_WHILE_M_, has_monad)(                                                \
      cond, body, id_while, id_if, has_defer, cap, __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_WHILE(cond, body) \
  DO_LET_IS_CXX_WORK_WHILE DO_LET_IS_DELAY_OPEN_BRACE_0() cond, body DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_WORK_CONTINUE_0(break_flag, id_while, id_if, cap, has_monad, ...) \
  return DO_LET_IS_CONCAT_0(while_self_, DO_LET_IS_DEC(id_while))(break_flag);

#define DO_LET_IS_CXX_WORK_CONTINUE_1(break_flag, id_while, id_if, cap, has_monad, ...)                  \
  {                                                                                                      \
    _do_loop_defer(false);                                                                               \
    return DO_LET_IS_CONCAT_0(while_self_, DO_LET_IS_DEC(id_while))(break_flag);                         \
  }

#define DO_LET_IS_CXX_WORK_CONTINUE_BASE(break_flag, id_while, id_if, has_defer, cap, has_monad, ...)    \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_CONTINUE_, has_defer)(                                               \
      break_flag, id_while, id_if, cap, has_monad __VA_OPT__(, ) __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_CONTINUE \
  DO_LET_IS_CXX_WORK_CONTINUE_BASE DO_LET_IS_DELAY_OPEN_BRACE_0() false DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_TABLE_BREAK \
  DO_LET_IS_CXX_WORK_CONTINUE_BASE DO_LET_IS_DELAY_OPEN_BRACE_0() true DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_WORK_END_IF_0(branch_id, id_while, id_if, cap, has_monad, ...) \
  return if_self_##branch_id(true);

#define DO_LET_IS_CXX_WORK_END_IF_1(branch_id, id_while, id_if, cap, has_monad, ...)                     \
  _do_if_defer(false);                                                                                   \
  return if_self_##branch_id(true);

#define DO_LET_IS_CXX_WORK_END_IF(branch_id, id_while, id_if, has_defer, cap, has_monad, ...)            \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_END_IF_, has_defer)(                                                 \
      branch_id, id_while, id_if, cap, has_monad __VA_OPT__(, ) __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_END_IF(branch_id) \
  DO_LET_IS_CXX_WORK_END_IF DO_LET_IS_DELAY_OPEN_BRACE_0() branch_id DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_CONT_MONAD_0(self_fn)
#define DO_LET_IS_CXX_CONT_MONAD_1(self_fn) \
  using _do_monad_t [[maybe_unused]] = std::remove_cvref_t<decltype(self_fn(true))>;

#define DO_LET_IS_CXX_WORK_IF_0(check, body, els, id_while, id_if, cap, has_monad, cont_let, ...)        \
  return [DO_LET_IS_CAP(cap)](this const auto& if_self_##id_if, bool is_cont = false) {                  \
    (void)if_self_##id_if;                                                                               \
    if (is_cont) {                                                                                       \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                \
          id_while, DO_LET_IS_INC(id_if), 0, cap, has_monad, __VA_ARGS__)                                \
    }                                                                                                    \
    DO_LET_IS_CAT2(DO_LET_IS_CXX_CONT_MONAD_, cont_let)(if_self_##id_if)                                 \
    if check {                                                                                           \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                \
          id_while, DO_LET_IS_INC(id_if), 0, cap, has_monad, DO_LET_IS_UNWRAP body, _END_IF(id_if))      \
    } else {                                                                                             \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                \
          id_while, DO_LET_IS_INC(id_if), 0, cap, has_monad, DO_LET_IS_UNWRAP els, _END_IF(id_if))       \
    }                                                                                                    \
  }();

#define DO_LET_IS_CXX_WORK_IF_1(check, body, els, id_while, id_if, cap, has_monad, cont_let, ...)        \
  return [DO_LET_IS_CAP(cap)](this const auto& if_self_##id_if, bool is_cont = false) {                  \
    (void)if_self_##id_if;                                                                               \
    if (is_cont) {                                                                                       \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                \
          id_while, DO_LET_IS_INC(id_if), 1, cap, has_monad, __VA_ARGS__)                                \
    }                                                                                                    \
    DO_LET_IS_CAT2(DO_LET_IS_CXX_CONT_MONAD_, cont_let)(if_self_##id_if)                                 \
    if check {                                                                                           \
      [[maybe_unused]] ::doletis::internal::noop_defer _do_if_defer{};                                   \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                \
          id_while, DO_LET_IS_INC(id_if), 1, cap, has_monad, DO_LET_IS_UNWRAP body, _END_IF(id_if))      \
    } else {                                                                                             \
      [[maybe_unused]] ::doletis::internal::noop_defer _do_if_defer{};                                   \
      DO_LET_IS_CXX_PARSE_HELPER DO_LET_IS_DELAY_OUT_4()(                                                \
          id_while, DO_LET_IS_INC(id_if), 1, cap, has_monad, DO_LET_IS_UNWRAP els, _END_IF(id_if))       \
    }                                                                                                    \
  }();

#define DO_LET_IS_CXX_WORK_IF_M0_H(tag, check, body, els, id_while, id_if, has_defer, cap, has_let, c_let, ...) \
  {                                                                                                             \
    DO_LET_IS_CAT2(DO_LET_IS_CXX_SCAN_MONAD_, has_let)(tag(check, body, els), __VA_ARGS__)                      \
    DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_IF_, has_defer)(                                                          \
        check, body, els, id_while, id_if, cap, has_let, c_let, __VA_ARGS__)                                    \
  }

#define DO_LET_IS_CXX_WORK_IF_M_0(tag, check, body, els, id_while, id_if, has_defer, cap, ...)           \
  DO_LET_IS_CXX_WORK_IF_M0_H(tag, check, body, els, id_while, id_if, has_defer, cap,                     \
                             DO_LET_IS_HAS_LET(tag(check, body, els), __VA_ARGS__),                      \
                             DO_LET_IS_HAS_LET(__VA_ARGS__), __VA_ARGS__)

#define DO_LET_IS_CXX_WORK_IF_M_1(tag, check, body, els, id_while, id_if, has_defer, cap, ...)           \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_IF_, has_defer)(                                                     \
      check, body, els, id_while, id_if, cap, 1, DO_LET_IS_HAS_LET(__VA_ARGS__), __VA_ARGS__)

#define DO_LET_IS_CXX_WORK_IF(tag, check, body, els, id_while, id_if, has_defer, cap, has_monad, ...)    \
  DO_LET_IS_CAT2(DO_LET_IS_CXX_WORK_IF_M_, has_monad)(                                                   \
      tag, check, body, els, id_while, id_if, has_defer, cap, __VA_ARGS__)

#define DO_LET_IS_CXX_TABLE_IF(check, body, els) \
  DO_LET_IS_CXX_WORK_IF DO_LET_IS_DELAY_OPEN_BRACE_0() _IF, check, body, els DO_LET_IS_CLOSE_MACRO
#define DO_LET_IS_CXX_TABLE_IFC(check, body, els) \
  DO_LET_IS_CXX_WORK_IF DO_LET_IS_DELAY_OPEN_BRACE_0() _IFC, check, body, els DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_CXX_PARSE_DO_ITERATION(id_while, id_if, has_defer, cap, has_monad, check, ...) \
  DO_LET_IS_CXX_TABLE##check(id_while, id_if, has_defer, cap, has_monad __VA_OPT__(, ) __VA_ARGS__)

#define DO_LET_IS_CXX_DO(...)                                                            \
  [&] {                                                                                  \
    using _do_monad_t [[maybe_unused]] = void;                                           \
    DO_LET_IS_EVAL(DO_LET_IS_CXX_PARSE_DO_ITERATION(0, 0, 0, 0, 0, _CODE(__VA_ARGS__)))  \
  }()

#define DO_LET_IS_CXX_DO_T(Ret, ...)                                                     \
  [&]() -> Ret {                                                                         \
    using _do_monad_t [[maybe_unused]] = Ret;                                            \
    DO_LET_IS_EVAL(DO_LET_IS_CXX_PARSE_DO_ITERATION(0, 0, 0, 0, 1, _CODE(__VA_ARGS__)))  \
  }()

#define DO_LET_IS_CXX_DO_GLOBAL(...)                                                     \
  [] {                                                                                   \
    using _do_monad_t [[maybe_unused]] = void;                                           \
    DO_LET_IS_EVAL(DO_LET_IS_CXX_PARSE_DO_ITERATION(0, 0, 0, 0, 0, _CODE(__VA_ARGS__)))  \
  }()

#define DO_LET_IS_HAVE_CXX_DO 1

namespace doletis {

template <typename Monad>
struct monad_tag {
  using type = Monad;
};

template <typename Monad>
struct monad_traits;

namespace internal {

template <typename T>
struct monad_probe {
  using type = T;
};

template <typename T>
auto make_monad_probe(T&&) -> monad_probe<std::remove_cvref_t<T>>;

template <typename T>
struct unwrap_monad_probe {
  using type = std::remove_cvref_t<T>;
};

template <typename T>
struct unwrap_monad_probe<monad_probe<T>> {
  using type = T;
};

template <typename A, typename B>
struct is_same_template : std::false_type {};

template <template <typename...> class Tmpl, typename... As, typename... Bs>
struct is_same_template<Tmpl<As...>, Tmpl<Bs...>> : std::true_type {};

template <typename Arg, typename OldElem, typename NewElem>
struct rebind_dependent_arg {
  using type = Arg;
};

template <template <typename...> class AllocTmpl, typename OldElem, typename NewElem>
  requires requires(AllocTmpl<OldElem> a) { a.allocate(0u); }
struct rebind_dependent_arg<AllocTmpl<OldElem>, OldElem, NewElem> {
  using type = AllocTmpl<NewElem>;
};

template <typename M, typename U>
struct rebind_monad {
  using type = M;
};

template <template <typename...> class Tmpl, typename First, typename... Rest, typename U>
struct rebind_monad<Tmpl<First, Rest...>, U> {
  using type = Tmpl<U, typename rebind_dependent_arg<Rest, First, U>::type...>;
};

template <typename M>
struct monad_first_arg {
  using type = void;
};

template <template <typename...> class Tmpl, typename First, typename... Rest>
struct monad_first_arg<Tmpl<First, Rest...>> {
  using type = First;
};

void doletis_bind() = delete;
void doletis_bind_defer() = delete;
void doletis_pure() = delete;
void doletis_is_err() = delete;

template <typename M, typename V>
constexpr decltype(auto) invoke_pure(V&& val) {
  if constexpr (std::is_void_v<M>) {
    return std::forward<V>(val);
  } else if constexpr (requires { ::doletis::monad_traits<M>::pure(std::forward<V>(val)); }) {
    return ::doletis::monad_traits<M>::pure(std::forward<V>(val));
  } else if constexpr (requires { doletis_pure(::doletis::monad_tag<M>{}, std::forward<V>(val)); }) {
    return doletis_pure(::doletis::monad_tag<M>{}, std::forward<V>(val));
  } else if constexpr (is_same_template<M, std::remove_cvref_t<V>>::value) {
    return std::forward<V>(val);
  } else if constexpr (!std::is_void_v<typename monad_first_arg<M>::type> &&
                       std::is_convertible_v<V, M> &&
                       !std::is_constructible_v<typename monad_first_arg<M>::type, V>) {
    return M(std::forward<V>(val));
  } else {
    using Rebound = typename rebind_monad<M, std::decay_t<V>>::type;
    return Rebound{std::forward<V>(val)};
  }
}

struct noop_defer {
  constexpr void operator()(bool) const noexcept {}
};

template <typename Fn, typename Prev>
struct defer_node {
  mutable Fn fn;
  Prev prev;
  bool only_err;
  constexpr void operator()(bool is_err) const {
    if (!only_err || is_err) {
      fn();
    }
    prev(is_err);
  }
};

template <typename Fn, typename Prev>
constexpr auto make_defer(bool only_err, Fn fn, Prev prev) {
  return defer_node<std::decay_t<Fn>, std::decay_t<Prev>>{std::move(fn), std::move(prev), only_err};
}

template <typename T>
constexpr bool is_err_value(const T& v) {
  using RawT = std::remove_cvref_t<T>;
  if constexpr (requires { ::doletis::monad_traits<RawT>::is_err(v); }) {
    return static_cast<bool>(::doletis::monad_traits<RawT>::is_err(v));
  } else if constexpr (requires { doletis_is_err(v); }) {
    return static_cast<bool>(doletis_is_err(v));
  } else if constexpr (requires { v.has_error(); }) {
    return static_cast<bool>(v.has_error());
  } else if constexpr (requires { v.has_value(); }) {
    return !static_cast<bool>(v.has_value());
  } else if constexpr (requires { !v; }) {
    return !v;
  } else {
    return false;
  }
}

template <typename F>
struct entered_guard {
  mutable F f;
  bool* entered;
  template <typename... Args>
  constexpr decltype(auto) operator()(Args&&... args) const {
    *entered = true;
    return f(std::forward<Args>(args)...);
  }
};

template <typename M, typename F>
constexpr decltype(auto) invoke_bind_0(M&& m, F&& f) {
  using RawM = std::remove_cvref_t<M>;
  if constexpr (requires { ::doletis::monad_traits<RawM>::bind(std::forward<M>(m), std::forward<F>(f)); }) {
    return ::doletis::monad_traits<RawM>::bind(std::forward<M>(m), std::forward<F>(f));
  } else {
    return doletis_bind(std::forward<M>(m), std::forward<F>(f));
  }
}

template <typename M, typename Defer, typename F>
constexpr decltype(auto) invoke_bind(M&& m, const Defer& defer, F&& f) {
  using RawM = std::remove_cvref_t<M>;
  if constexpr (requires {
                  ::doletis::monad_traits<RawM>::bind_defer(std::forward<M>(m), defer, std::forward<F>(f));
                }) {
    return ::doletis::monad_traits<RawM>::bind_defer(std::forward<M>(m), defer, std::forward<F>(f));
  } else if constexpr (requires { doletis_bind_defer(std::forward<M>(m), defer, std::forward<F>(f)); }) {
    return doletis_bind_defer(std::forward<M>(m), defer, std::forward<F>(f));
  } else if constexpr (std::is_same_v<std::decay_t<Defer>, noop_defer>) {
    return invoke_bind_0(std::forward<M>(m), std::forward<F>(f));
  } else {
    bool entered = false;
    auto res = invoke_bind_0(
        std::forward<M>(m), entered_guard<std::decay_t<F>>{std::forward<F>(f), &entered});
    if (!entered) {
      defer(true);
    }
    return res;
  }
}

}  // namespace internal
}  // namespace doletis

#endif  // DO_LET_IS_CXX_DO_LET_IS_H

#ifdef DO_LET_IS_USE_BLOCKS
#ifndef DO_LET_IS_C_DO_LET_IS_H
#include "do_let_is/c/do_let_is.h"
#endif
#define DO_LET_IS_NEXT_SHORT_IS_C 1
#ifndef DO_LET_IS_DO
#define DO_LET_IS_DO DO_LET_IS_C_DO
#define DO_LET_IS_DO_T DO_LET_IS_C_DO_T
#define DO_LET_IS_DO_GLOBAL DO_LET_IS_C_DO_GLOBAL
#endif
#elif !defined(DO_LET_IS_INTERNAL_BRIDGE)
#undef DO_LET_IS_NEXT_SHORT_IS_C
#ifndef DO_LET_IS_DO
#define DO_LET_IS_DO DO_LET_IS_CXX_DO
#define DO_LET_IS_DO_T DO_LET_IS_CXX_DO_T
#define DO_LET_IS_DO_GLOBAL DO_LET_IS_CXX_DO_GLOBAL
#endif
#endif

#endif  // __cplusplus
