#include "do_let_is/c/do_let_is.h"

#ifndef DO_LET_IS_C_GENERATOR_H
#define DO_LET_IS_C_GENERATOR_H

#define DOLETIS_DEFINE_GENERATOR(T, Name)                                               \
  typedef struct Name##_yield_t {                                                       \
    T value;                                                                            \
  } Name##_yield_t;                                                                     \
  typedef struct Name##_step Name##_step;                                               \
  struct Name##_step {                                                                  \
    bool has_value;                                                                     \
    T value;                                                                            \
    Name##_step (^next)(void);                                                          \
    doletis_defer_t on_close;                                                           \
  };                                                                                    \
  typedef struct Name {                                                                 \
    Name##_step (^start)(void);                                                         \
    Name##_step current;                                                                \
    bool started;                                                                       \
    bool done;                                                                          \
  } Name;                                                                               \
  static inline DO_LET_IS_UNUSED Name##_step Name##_end(void) {                         \
    Name##_step _s = {};                                                                \
    return _s;                                                                          \
  }                                                                                     \
  DO_LET_IS_OVERLOAD int doletis_elem(Name##_yield_t);                                  \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name##_step doletis_pure(           \
      Name##_yield_t _t, Name##_step s) {                                               \
    (void)_t;                                                                           \
    return s;                                                                           \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name##_step doletis_pure(           \
      doletis_no_monad_t _t, Name##_step s) {                                           \
    (void)_t;                                                                           \
    return s;                                                                           \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name##_step doletis_bind(           \
      Name##_yield_t y, Name##_step (^k)(int)) {                                        \
    doletis_defer_t on_close = doletis_claim_defer();                                   \
    Name##_step (^k_copy)(int) = Block_copy(k);                                         \
    Name##_step (^next_blk)(void) = Block_copy(^ Name##_step (void) {                   \
      Name##_step s = k_copy(0);                                                        \
      Block_release(k_copy);                                                            \
      return s;                                                                         \
    });                                                                                 \
    Name##_step _out = {};                                                              \
    _out.has_value = true;                                                              \
    _out.value = y.value;                                                               \
    _out.next = next_blk;                                                               \
    _out.on_close = on_close;                                                           \
    return _out;                                                                        \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED bool Name##_next(Name* g, T* out) {                    \
    if (g->done) {                                                                      \
      return false;                                                                     \
    }                                                                                   \
    if (!g->started) {                                                                  \
      g->started = true;                                                                \
      Name##_step (^s_blk)(void) = g->start;                                            \
      g->start = nullptr;                                                               \
      g->current = s_blk();                                                             \
      Block_release(s_blk);                                                             \
    } else {                                                                            \
      Name##_step (^n_blk)(void) = g->current.next;                                     \
      doletis_defer_t oc = g->current.on_close;                                         \
      g->current.next = nullptr;                                                        \
      g->current.on_close = nullptr;                                                    \
      if (oc != nullptr) {                                                              \
        Block_release(oc);                                                              \
      }                                                                                 \
      if (n_blk == nullptr) {                                                           \
        g->done = true;                                                                 \
        return false;                                                                   \
      }                                                                                 \
      g->current = n_blk();                                                             \
      Block_release(n_blk);                                                             \
    }                                                                                   \
    if (!g->current.has_value) {                                                        \
      if (g->current.next != nullptr) {                                                 \
        Block_release(g->current.next);                                                 \
        g->current.next = nullptr;                                                      \
      }                                                                                 \
      if (g->current.on_close != nullptr) {                                             \
        Block_release(g->current.on_close);                                             \
        g->current.on_close = nullptr;                                                  \
      }                                                                                 \
      g->done = true;                                                                   \
      return false;                                                                     \
    }                                                                                   \
    if (out != nullptr) {                                                               \
      *out = g->current.value;                                                          \
    }                                                                                   \
    return true;                                                                        \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_close(Name* g) {                           \
    if (g->start != nullptr) {                                                          \
      Block_release(g->start);                                                          \
      g->start = nullptr;                                                               \
    }                                                                                   \
    if (g->current.on_close != nullptr) {                                               \
      doletis_defer_t oc = g->current.on_close;                                         \
      g->current.on_close = nullptr;                                                    \
      oc(false);                                                                        \
      Block_release(oc);                                                                \
    }                                                                                   \
    if (g->current.next != nullptr) {                                                   \
      Block_release(g->current.next);                                                   \
      g->current.next = nullptr;                                                        \
    }                                                                                   \
    g->done = true;                                                                     \
  }

#define DO_LET_IS_C_GENERATOR(GenName, ...)                                                   \
  (__extension__({                                                                            \
    GenName _do_gen = {};                                                                     \
    _do_gen.start = Block_copy(^ GenName##_step (void) {                                      \
      typedef GenName##_yield_t _do_monad_t DO_LET_IS_UNUSED;                                 \
      typedef GenName##_step _do_ret_t DO_LET_IS_UNUSED;                                      \
      typedef GenName##_yield_t _do_yield_t DO_LET_IS_UNUSED;                                 \
      DO_LET_IS_C_INIT_DEFER_SCOPE()                                                          \
      DO_LET_IS_EVAL(DO_LET_IS_C_PARSE_DO_ITERATION(0, 0, _CODE(__VA_ARGS__)))                \
    });                                                                                       \
    _do_gen;                                                                                  \
  }))

#define DO_LET_IS_C_YIELD(...) \
  DO_LET_IS_LET _do_y DO_LET_IS_IS(__extension__({ _do_yield_t _yv = {}; _yv.value = (__VA_ARGS__); _yv; }))

#define DO_LET_IS_HAVE_C_GENERATOR 1

#endif  // DO_LET_IS_C_GENERATOR_H

#ifndef DO_LET_IS_GENERATOR
#define DO_LET_IS_GENERATOR DO_LET_IS_C_GENERATOR
#define DO_LET_IS_YIELD DO_LET_IS_C_YIELD
#endif
