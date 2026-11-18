#if !defined(__BLOCKS__)
#error "do_let_is blocks backend requires Clang blocks support (-fblocks)"
#endif

#if defined(__cplusplus) && !defined(DO_LET_IS_CXX_DO_LET_IS_H)
#define DO_LET_IS_INTERNAL_BRIDGE 1
#include "do_let_is/do_let_is.h"
#undef DO_LET_IS_INTERNAL_BRIDGE
#endif

#ifndef DO_LET_IS_C_DO_LET_IS_H
#define DO_LET_IS_C_DO_LET_IS_H

#include <Block.h>

#include "do_let_is/common.h"

typedef __typeof__(sizeof(0)) doletis_usize_t;

#if !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L)
#include <stdbool.h>
#ifndef nullptr
#define nullptr ((void*)0)
#endif
#ifndef thread_local
#define thread_local _Thread_local
#endif
#ifndef auto
#define auto __auto_type
#endif
#endif

#define DO_LET_IS_OVERLOAD __attribute__((overloadable))
#define DO_LET_IS_UNUSED __attribute__((unused))

typedef void (^doletis_defer_t)(bool);
typedef void (^doletis_any_block_t)(void);

typedef struct {
  unsigned char _unused;
} doletis_no_monad_t;

static inline DO_LET_IS_UNUSED doletis_defer_t doletis_noop_defer(void) {
  return ^ void (bool _do_e) {
    (void)_do_e;
  };
}

static thread_local doletis_defer_t doletis_tls_defer_ = nullptr;
static thread_local bool* doletis_tls_claimed_ = nullptr;

static inline DO_LET_IS_UNUSED doletis_defer_t doletis_claim_defer(void) {
  if (doletis_tls_claimed_ != nullptr) {
    *doletis_tls_claimed_ = true;
  }
  if (doletis_tls_defer_ != nullptr) {
    return Block_copy(doletis_tls_defer_);
  }
  return nullptr;
}

DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED bool doletis_is_err_impl(int _do_tag, ...) {
  (void)_do_tag;
  return false;
}

#ifdef __cplusplus
namespace doletis::c_bridge {

void doletis_elem() = delete;
void doletis_bind() = delete;
void doletis_pure() = delete;
void doletis_is_err_impl() = delete;

template <typename Blk>
struct block_fn {
  Blk blk;
  template <typename... Args>
  constexpr decltype(auto) operator()(Args&&... args) const {
    return blk(std::forward<Args>(args)...);
  }
};

template <typename M>
consteval auto deduce_elem() {
  using RawM = std::remove_cvref_t<M>;
  if constexpr (requires(RawM m) { doletis_elem(m); }) {
    return std::type_identity<decltype(doletis_elem(std::declval<RawM>()))>{};
  } else if constexpr (requires { typename ::doletis::monad_traits<RawM>::value_type; }) {
    return std::type_identity<typename ::doletis::monad_traits<RawM>::value_type>{};
  } else if constexpr (requires { typename RawM::value_type; }) {
    return std::type_identity<typename RawM::value_type>{};
  } else {
    return std::type_identity<typename ::doletis::internal::monad_first_arg<RawM>::type>{};
  }
}

template <typename M>
using elem_t = typename decltype(deduce_elem<M>())::type;

template <typename M, typename Blk>
decltype(auto) call_bind(M&& m, Blk blk) {
  using RawM = std::remove_cvref_t<M>;
  if constexpr (requires(RawM rm) { doletis_elem(rm); }) {
    return doletis_bind(std::forward<M>(m), blk);
  } else {
    return ::doletis::internal::invoke_bind_0(std::forward<M>(m), block_fn<Blk>{blk});
  }
}

template <typename MonadT, typename V>
decltype(auto) call_pure(V&& v) {
  if constexpr (std::is_same_v<MonadT, ::doletis_no_monad_t>) {
    return std::forward<V>(v);
  } else if constexpr (requires(MonadT tag) { doletis_pure(tag, std::forward<V>(v)); }) {
    MonadT tag = {};
    return doletis_pure(tag, std::forward<V>(v));
  } else {
    return ::doletis::internal::invoke_pure<MonadT>(std::forward<V>(v));
  }
}

template <typename T>
bool call_is_err(const T& v) {
  using RawT = std::remove_cvref_t<T>;
  if constexpr (requires(RawT rt) { doletis_elem(rt); }) {
    return doletis_is_err_impl(0, v);
  } else {
    return ::doletis::internal::is_err_value(v);
  }
}

}  // namespace doletis::c_bridge

#define DO_LET_IS_C_ELEM_TYPE(m) ::doletis::c_bridge::elem_t<decltype(m)>
#define DO_LET_IS_C_CALL_BIND(m, blk) ::doletis::c_bridge::call_bind((m), (blk))
#define DO_LET_IS_C_CALL_PURE(MonadT, ...) ::doletis::c_bridge::call_pure<MonadT>((__VA_ARGS__))
#define DO_LET_IS_C_IS_ERR(x) ::doletis::c_bridge::call_is_err((x))
#else
#define DO_LET_IS_C_ELEM_TYPE(m) __typeof__(doletis_elem(m))
#define DO_LET_IS_C_CALL_BIND(m, blk) doletis_bind((m), (blk))
#define DO_LET_IS_C_CALL_PURE(MonadT, ...) \
  __extension__({                          \
    MonadT _do_mtag = {};                  \
    doletis_pure(_do_mtag, (__VA_ARGS__)); \
  })
#define DO_LET_IS_C_IS_ERR(x) doletis_is_err_impl(0, (x))
#endif

#define DO_LET_IS_C_PARSE_HELPER(...) DO_LET_IS_C_PARSE_DO_ITERATION DO_LET_IS_OUT(__VA_ARGS__)

#define DO_LET_IS_C_ROOT_MONAD_0(...)                     \
  typedef doletis_no_monad_t _do_monad_t DO_LET_IS_UNUSED;
#define DO_LET_IS_C_ROOT_MONAD_1(...)                                            \
  _Pragma("GCC diagnostic push")                                                 \
  _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")                        \
  _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"")                \
  _Pragma("GCC diagnostic ignored \"-Wunused-value\"")                           \
  typedef __typeof__(__extension__({                                             \
    DO_LET_IS_SCAN_EVAL(DO_LET_IS_SCAN_STEP(C, __VA_ARGS__, _END_SCAN))          \
  })) _do_monad_t DO_LET_IS_UNUSED;                                              \
  _Pragma("GCC diagnostic pop")

#define DO_LET_IS_C_INIT_ROOT_MONAD(...) \
  DO_LET_IS_CAT2(DO_LET_IS_C_ROOT_MONAD_, DO_LET_IS_HAS_LET(__VA_ARGS__))(__VA_ARGS__)

#define DO_LET_IS_C_INIT_DEFER_SCOPE()                   \
  doletis_defer_t _do_defer = doletis_noop_defer();      \
  doletis_defer_t _do_loop_defer = doletis_noop_defer(); \
  doletis_defer_t _do_if_defer = doletis_noop_defer();   \
  (void)_do_defer;                                       \
  (void)_do_loop_defer;                                  \
  (void)_do_if_defer;

#define DO_LET_IS_C_WORK_LET_IS(name, is, id_while, id_if, ...)                                   \
  return __extension__({                                                                          \
    auto _do_m = (DO_LET_IS_UNWRAP is);                                                           \
    __block bool _do_entered = false;                                                             \
    bool _do_claimed = false;                                                                     \
    doletis_defer_t _do_prev_tls = doletis_tls_defer_;                                            \
    bool* _do_prev_claimed = doletis_tls_claimed_;                                                \
    doletis_tls_defer_ = _do_defer;                                                               \
    doletis_tls_claimed_ = &_do_claimed;                                                          \
    auto _do_res = DO_LET_IS_C_CALL_BIND(_do_m, ^(DO_LET_IS_C_ELEM_TYPE(_do_m) name) {            \
      typedef __typeof__(_do_m) _do_monad_t DO_LET_IS_UNUSED;                                     \
      (void)name;                                                                                 \
      _do_entered = true;                                                                         \
      DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, id_if, __VA_ARGS__)              \
    });                                                                                           \
    doletis_tls_defer_ = _do_prev_tls;                                                            \
    doletis_tls_claimed_ = _do_prev_claimed;                                                      \
    if (!_do_entered && !_do_claimed) {                                                           \
      _do_defer(true);                                                                            \
    }                                                                                             \
    _do_res;                                                                                      \
  });

#define DO_LET_IS_C_TABLE_LET_IS(name, is) \
  DO_LET_IS_C_WORK_LET_IS DO_LET_IS_DELAY_OPEN_BRACE_0() name, is DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_C_WORK_DEFER_IMPL(cond_expr, cleanup, id_while, id_if, ...)                     \
  {                                                                                               \
    doletis_defer_t _do_prev_all = _do_defer;                                                     \
    doletis_defer_t _do_prev_loop = _do_loop_defer;                                               \
    doletis_defer_t _do_prev_if = _do_if_defer;                                                   \
    doletis_defer_t _do_step = ^ void (bool _do_is_err) {                                         \
      (void)_do_is_err;                                                                           \
      if (cond_expr) {                                                                            \
        DO_LET_IS_UNWRAP cleanup;                                                                 \
      }                                                                                           \
    };                                                                                            \
    {                                                                                             \
      doletis_defer_t _do_defer = ^ void (bool _do_e) {                                           \
        _do_step(_do_e);                                                                          \
        _do_prev_all(_do_e);                                                                      \
      };                                                                                          \
      doletis_defer_t _do_loop_defer = ^ void (bool _do_e) {                                      \
        _do_step(_do_e);                                                                          \
        _do_prev_loop(_do_e);                                                                     \
      };                                                                                          \
      doletis_defer_t _do_if_defer = ^ void (bool _do_e) {                                        \
        _do_step(_do_e);                                                                          \
        _do_prev_if(_do_e);                                                                       \
      };                                                                                          \
      (void)_do_defer;                                                                            \
      (void)_do_loop_defer;                                                                       \
      (void)_do_if_defer;                                                                         \
      DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, id_if, __VA_ARGS__)              \
    }                                                                                             \
  }

#define DO_LET_IS_C_WORK_DEFER(cleanup, id_while, id_if, ...) \
  DO_LET_IS_C_WORK_DEFER_IMPL(true, cleanup, id_while, id_if, __VA_ARGS__)

#define DO_LET_IS_C_WORK_ERRDEFER(cleanup, id_while, id_if, ...) \
  DO_LET_IS_C_WORK_DEFER_IMPL(_do_is_err, cleanup, id_while, id_if, __VA_ARGS__)

#define DO_LET_IS_C_TABLE_DEFER(cleanup) \
  DO_LET_IS_C_WORK_DEFER DO_LET_IS_DELAY_OPEN_BRACE_0() cleanup DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_C_TABLE_ERRDEFER(cleanup) \
  DO_LET_IS_C_WORK_ERRDEFER DO_LET_IS_DELAY_OPEN_BRACE_0() cleanup DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_C_WORK_RETURN(expr, id_while, id_if, ...)                                       \
  {                                                                                               \
    auto _do_ret = DO_LET_IS_C_CALL_PURE(_do_monad_t, DO_LET_IS_UNWRAP expr);                     \
    _do_defer(DO_LET_IS_C_IS_ERR(_do_ret));                                                       \
    return _do_ret;                                                                               \
  }

#define DO_LET_IS_C_TABLE_RETURN(expr) \
  DO_LET_IS_C_WORK_RETURN DO_LET_IS_DELAY_OPEN_BRACE_0() expr DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_C_WORK_CODE(id_while, id_if, ...) \
  __VA_OPT__(DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_0()(id_while, id_if, __VA_ARGS__))

#define DO_LET_IS_C_TABLE_CODE(...) __VA_ARGS__ DO_LET_IS_C_WORK_CODE

#define DO_LET_IS_C_CALL_WHILE_1(id, flag) \
  ((_do_wblk_##id)(void*)_do_wself_##id)(_do_wself_##id, (flag))
#define DO_LET_IS_C_CALL_WHILE(id, flag) DO_LET_IS_C_CALL_WHILE_1(id, flag)

#define DO_LET_IS_C_WORK_WHILE(cond, body, id_while, id_if, ...)                                  \
  {                                                                                               \
    typedef _do_ret_t (^_do_wblk_##id_while)(doletis_any_block_t, bool);                          \
    _do_wblk_##id_while _do_wfn_##id_while =                                                      \
        ^ _do_ret_t (doletis_any_block_t _do_wself_##id_while, bool _do_brk) {                    \
      (void)_do_wself_##id_while;                                                                 \
      if (_do_brk || !cond) {                                                                     \
        DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_3()(id_while, id_if, __VA_ARGS__)            \
      }                                                                                           \
      {                                                                                           \
        doletis_defer_t _do_loop_defer = doletis_noop_defer();                                    \
        doletis_defer_t _do_if_defer = doletis_noop_defer();                                      \
        (void)_do_loop_defer;                                                                     \
        (void)_do_if_defer;                                                                       \
        DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_3()(                                         \
            DO_LET_IS_INC(id_while), id_if, DO_LET_IS_UNWRAP body,                                \
            _CODE(_do_loop_defer(false);                                                          \
                  return ((_do_wblk_##id_while)(void*)_do_wself_##id_while)(                      \
                      _do_wself_##id_while, false);))                                             \
      }                                                                                           \
    };                                                                                            \
    return _do_wfn_##id_while((doletis_any_block_t)(void*)_do_wfn_##id_while, false);             \
  }

#define DO_LET_IS_C_TABLE_WHILE(cond, body) \
  DO_LET_IS_C_WORK_WHILE DO_LET_IS_DELAY_OPEN_BRACE_0() cond, body DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_C_WORK_CONTINUE_BASE(break_flag, id_while, id_if, ...)                          \
  {                                                                                               \
    _do_loop_defer(false);                                                                        \
    return DO_LET_IS_C_CALL_WHILE(DO_LET_IS_DEC(id_while), break_flag);                           \
  }

#define DO_LET_IS_C_TABLE_CONTINUE \
  DO_LET_IS_C_WORK_CONTINUE_BASE DO_LET_IS_DELAY_OPEN_BRACE_0() false DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_C_TABLE_BREAK \
  DO_LET_IS_C_WORK_CONTINUE_BASE DO_LET_IS_DELAY_OPEN_BRACE_0() true DO_LET_IS_CLOSE_MACRO

#define DO_LET_IS_C_WORK_IF(check, body, els, id_while, id_if, ...)                               \
  {                                                                                               \
    typedef _do_ret_t (^_do_iblk_##id_if)(doletis_any_block_t, bool);                             \
    _do_iblk_##id_if _do_ifn_##id_if =                                                            \
        ^ _do_ret_t (doletis_any_block_t _do_iself_##id_if, bool _do_is_cont) {                   \
      (void)_do_iself_##id_if;                                                                    \
      if (_do_is_cont) {                                                                          \
        DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_3()(                                         \
            id_while, DO_LET_IS_INC(id_if), __VA_ARGS__)                                          \
      }                                                                                           \
      if check {                                                                                  \
        doletis_defer_t _do_if_defer = doletis_noop_defer();                                      \
        (void)_do_if_defer;                                                                       \
        DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_3()(                                         \
            id_while, DO_LET_IS_INC(id_if), DO_LET_IS_UNWRAP body,                                \
            _CODE(_do_if_defer(false);                                                            \
                  return ((_do_iblk_##id_if)(void*)_do_iself_##id_if)(_do_iself_##id_if, true);)) \
      } else {                                                                                    \
        doletis_defer_t _do_if_defer = doletis_noop_defer();                                      \
        (void)_do_if_defer;                                                                       \
        DO_LET_IS_C_PARSE_HELPER DO_LET_IS_DELAY_OUT_3()(                                         \
            id_while, DO_LET_IS_INC(id_if), DO_LET_IS_UNWRAP els,                                 \
            _CODE(_do_if_defer(false);                                                            \
                  return ((_do_iblk_##id_if)(void*)_do_iself_##id_if)(_do_iself_##id_if, true);)) \
      }                                                                                           \
    };                                                                                            \
    return _do_ifn_##id_if((doletis_any_block_t)(void*)_do_ifn_##id_if, false);                   \
  }

#define DO_LET_IS_C_TABLE_IF(check, body, els) \
  DO_LET_IS_C_WORK_IF DO_LET_IS_DELAY_OPEN_BRACE_0() check, body, els DO_LET_IS_CLOSE_MACRO
#define DO_LET_IS_C_TABLE_IFC DO_LET_IS_C_TABLE_IF

#define DO_LET_IS_C_PARSE_DO_ITERATION(id_while, id_if, check, ...) \
  DO_LET_IS_C_TABLE##check(id_while, id_if __VA_OPT__(, ) __VA_ARGS__)

#define DO_LET_IS_C_DO(...)                                                  \
  (^{                                                                        \
    DO_LET_IS_C_INIT_ROOT_MONAD(_CODE(__VA_ARGS__))                          \
    typedef _do_monad_t _do_ret_t DO_LET_IS_UNUSED;                          \
    DO_LET_IS_C_INIT_DEFER_SCOPE()                                           \
    DO_LET_IS_EVAL(DO_LET_IS_C_PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))) \
  }())

#define DO_LET_IS_C_DO_T(Ret, ...)                                           \
  (^ Ret (void) {                                                            \
    DO_LET_IS_C_INIT_ROOT_MONAD(_CODE(__VA_ARGS__))                          \
    typedef Ret _do_ret_t DO_LET_IS_UNUSED;                                  \
    DO_LET_IS_C_INIT_DEFER_SCOPE()                                           \
    DO_LET_IS_EVAL(DO_LET_IS_C_PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__))) \
  }())

#define DO_LET_IS_C_DO_GLOBAL(...) DO_LET_IS_C_DO(__VA_ARGS__)

#define DO_LET_IS_HAVE_C_DO 1

#endif  // DO_LET_IS_C_DO_LET_IS_H

#define DO_LET_IS_NEXT_SHORT_IS_C 1
#ifndef DO_LET_IS_DO
#define DO_LET_IS_DO DO_LET_IS_C_DO
#define DO_LET_IS_DO_T DO_LET_IS_C_DO_T
#define DO_LET_IS_DO_GLOBAL DO_LET_IS_C_DO_GLOBAL
#endif
