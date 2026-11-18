#include "do_let_is/c/do_let_is.h"

#ifndef DO_LET_IS_C_MONADS_H
#define DO_LET_IS_C_MONADS_H

typedef struct {
  unsigned char _unused;
} doletis_unit_t;

static inline DO_LET_IS_UNUSED doletis_unit_t doletis_make_unit(void) {
  doletis_unit_t _u = {};
  return _u;
}

#define DOLETIS_UNIT (doletis_make_unit())

#define DOLETIS_DEFINE_OPTIONAL(T, Name)                                                 \
  typedef struct Name {                                                                  \
    bool has_value;                                                                      \
    T value;                                                                             \
  } Name;                                                                                \
  static inline DO_LET_IS_UNUSED Name Name##_some(T v) {                                 \
    Name _r = {};                                                                        \
    _r.has_value = true;                                                                 \
    _r.value = v;                                                                        \
    return _r;                                                                           \
  }                                                                                      \
  static inline DO_LET_IS_UNUSED Name Name##_none(void) {                                \
    Name _r = {};                                                                        \
    _r.has_value = false;                                                                \
    return _r;                                                                           \
  }                                                                                      \
  DO_LET_IS_OVERLOAD T doletis_elem(Name);                                               \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(Name _t, T v) {    \
    (void)_t;                                                                            \
    return Name##_some(v);                                                               \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(Name _t, Name m) { \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(                   \
      doletis_no_monad_t _t, Name m) {                                                   \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED bool doletis_is_err_impl(            \
      int _tag, Name m) {                                                                \
    (void)_tag;                                                                          \
    return !m.has_value;                                                                 \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_bind(                   \
      Name m, Name (^f)(T)) {                                                            \
    return m.has_value ? f(m.value) : Name##_none();                                     \
  }

#define DOLETIS_DEFINE_OPTIONAL_CONV(FromName, FromT, ToName)                            \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                 \
      FromName _t, __typeof__(doletis_elem(ToName##_none())) v) {                        \
    (void)_t;                                                                            \
    return ToName##_some(v);                                                             \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                 \
      FromName _t, ToName m) {                                                           \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_bind(                 \
      FromName m, ToName (^f)(FromT)) {                                                  \
    return m.has_value ? f(m.value) : ToName##_none();                                   \
  }

#define DOLETIS_DEFINE_EXPECTED(T, E, Name)                                              \
  typedef struct Name {                                                                  \
    bool has_value;                                                                      \
    union {                                                                              \
      T value;                                                                           \
      E error;                                                                           \
    };                                                                                   \
  } Name;                                                                                \
  static inline DO_LET_IS_UNUSED Name Name##_ok(T v) {                                   \
    Name _r = {};                                                                        \
    _r.has_value = true;                                                                 \
    _r.value = v;                                                                        \
    return _r;                                                                           \
  }                                                                                      \
  static inline DO_LET_IS_UNUSED Name Name##_err(E e) {                                  \
    Name _r = {};                                                                        \
    _r.has_value = false;                                                                \
    _r.error = e;                                                                        \
    return _r;                                                                           \
  }                                                                                      \
  DO_LET_IS_OVERLOAD T doletis_elem(Name);                                               \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(Name _t, T v) {    \
    (void)_t;                                                                            \
    return Name##_ok(v);                                                                 \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(Name _t, Name m) { \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(                   \
      doletis_no_monad_t _t, Name m) {                                                   \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED bool doletis_is_err_impl(            \
      int _tag, Name m) {                                                                \
    (void)_tag;                                                                          \
    return !m.has_value;                                                                 \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_bind(                   \
      Name m, Name (^f)(T)) {                                                            \
    return m.has_value ? f(m.value) : Name##_err(m.error);                               \
  }

#define DOLETIS_DEFINE_EXPECTED_CONV(FromName, FromT, ToName)                            \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                 \
      FromName _t, __typeof__(doletis_elem(*(ToName*)0)) v) {                            \
    (void)_t;                                                                            \
    return ToName##_ok(v);                                                               \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                 \
      FromName _t, ToName m) {                                                           \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_bind(                 \
      FromName m, ToName (^f)(FromT)) {                                                  \
    return m.has_value ? f(m.value) : ToName##_err(m.error);                             \
  }

#define DOLETIS_DEFINE_VEC(T, Cap, Name)                                                 \
  typedef struct Name {                                                                  \
    doletis_usize_t size;                                                                \
    T data[Cap];                                                                         \
  } Name;                                                                                \
  DO_LET_IS_OVERLOAD T doletis_elem(Name);                                               \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(Name _t, T v) {    \
    (void)_t;                                                                            \
    Name _r = {};                                                                        \
    _r.size = 1;                                                                         \
    _r.data[0] = v;                                                                      \
    return _r;                                                                           \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(Name _t, Name m) { \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(                   \
      doletis_no_monad_t _t, Name m) {                                                   \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_bind(                   \
      Name m, Name (^f)(T)) {                                                            \
    Name out = {};                                                                       \
    for (doletis_usize_t _i = 0; _i < m.size; ++_i) {                                    \
      Name part = f(m.data[_i]);                                                         \
      for (doletis_usize_t _j = 0; _j < part.size && out.size < (Cap); ++_j) {           \
        out.data[out.size++] = part.data[_j];                                            \
      }                                                                                  \
    }                                                                                    \
    return out;                                                                          \
  }

#define DOLETIS_DEFINE_VEC_CONV(FromName, FromT, ToName, Cap)                            \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                 \
      FromName _t, __typeof__(doletis_elem(*(ToName*)0)) v) {                            \
    (void)_t;                                                                            \
    ToName _r = {};                                                                      \
    _r.size = 1;                                                                         \
    _r.data[0] = v;                                                                      \
    return _r;                                                                           \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                 \
      FromName _t, ToName m) {                                                           \
    (void)_t;                                                                            \
    return m;                                                                            \
  }                                                                                      \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_bind(                 \
      FromName m, ToName (^f)(FromT)) {                                                  \
    ToName out = {};                                                                     \
    for (doletis_usize_t _i = 0; _i < m.size; ++_i) {                                    \
      ToName part = f(m.data[_i]);                                                       \
      for (doletis_usize_t _j = 0; _j < part.size && out.size < (Cap); ++_j) {           \
        out.data[out.size++] = part.data[_j];                                            \
      }                                                                                  \
    }                                                                                    \
    return out;                                                                          \
  }

#endif
