#include "do_let_is/c/do_let_is.h"

#ifndef DO_LET_IS_C_ASYNC_H
#define DO_LET_IS_C_ASYNC_H

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

typedef enum {
  DOLETIS_TASK_PENDING = 0,
  DOLETIS_TASK_READY = 1,
  DOLETIS_TASK_CANCELLED = 2,
} doletis_task_state_t;

typedef struct doletis_job {
  void (^run)(void);
  struct doletis_job* next;
} doletis_job;

typedef struct doletis_executor {
  doletis_job* head;
  doletis_job* tail;
} doletis_executor;

static inline DO_LET_IS_UNUSED void doletis_executor_init(doletis_executor* ex) {
  ex->head = nullptr;
  ex->tail = nullptr;
}

static inline DO_LET_IS_UNUSED void doletis_executor_post(doletis_executor* ex, void (^fn)(void)) {
  doletis_job* job = (doletis_job*)malloc(sizeof(doletis_job));
  job->run = Block_copy(fn);
  job->next = nullptr;
  if (ex->tail != nullptr) {
    ex->tail->next = job;
  } else {
    ex->head = job;
  }
  ex->tail = job;
}

static inline DO_LET_IS_UNUSED doletis_usize_t doletis_executor_run(doletis_executor* ex) {
  doletis_usize_t ran = 0;
  while (ex->head != nullptr) {
    doletis_job* job = ex->head;
    ex->head = job->next;
    if (ex->head == nullptr) {
      ex->tail = nullptr;
    }
    job->run();
    Block_release(job->run);
    free(job);
    ++ran;
  }
  return ran;
}

#ifndef DOLETIS_TRAMPOLINE_LIMIT
#define DOLETIS_TRAMPOLINE_LIMIT 32u
#endif

static thread_local unsigned doletis_sync_depth_ = 0;
static thread_local bool doletis_in_trampoline_ = false;
static thread_local doletis_executor doletis_trampoline_q_ = {nullptr, nullptr};

static inline DO_LET_IS_UNUSED void doletis_drain_trampoline(void) {
  if (doletis_sync_depth_ == 0 && !doletis_in_trampoline_ && doletis_trampoline_q_.head != nullptr) {
    doletis_in_trampoline_ = true;
    doletis_executor_run(&doletis_trampoline_q_);
    doletis_in_trampoline_ = false;
  }
}

#define DOLETIS_DEFINE_TASK(T, Name)                                                    \
  typedef struct Name##_box Name##_box;                                                 \
  struct Name##_box {                                                                   \
    int refs;                                                                           \
    doletis_task_state_t status;                                                        \
    T value;                                                                            \
    void (^on_complete)(T, bool);                                                       \
    Name##_box* forward_to;                                                             \
    void* cancel_ctx;                                                                   \
    void (*cancel_fn)(void*);                                                           \
    void (*cancel_drop)(void*);                                                         \
  };                                                                                    \
  typedef struct Name {                                                                 \
    Name##_box* box;                                                                    \
    T immediate;                                                                        \
    bool cancelled;                                                                     \
  } Name;                                                                               \
  typedef struct Name##_promise {                                                       \
    Name##_box* box;                                                                    \
  } Name##_promise;                                                                     \
  static inline DO_LET_IS_UNUSED Name##_box* Name##_box_new(void) {                     \
    Name##_box* b = (Name##_box*)calloc(1, sizeof(Name##_box));                         \
    b->refs = 1;                                                                        \
    b->status = DOLETIS_TASK_PENDING;                                                   \
    return b;                                                                           \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED Name##_box* Name##_box_retain(Name##_box* b) {         \
    if (b != nullptr) {                                                                 \
      b->refs += 1;                                                                     \
    }                                                                                   \
    return b;                                                                           \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_clear_cancel(Name##_box* b) {              \
    if (b->cancel_ctx != nullptr) {                                                     \
      void* ctx = b->cancel_ctx;                                                        \
      void (*drop)(void*) = b->cancel_drop;                                             \
      b->cancel_ctx = nullptr;                                                          \
      b->cancel_fn = nullptr;                                                           \
      b->cancel_drop = nullptr;                                                         \
      if (drop != nullptr) {                                                            \
        drop(ctx);                                                                      \
      }                                                                                 \
    }                                                                                   \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_box_release(Name##_box* b) {               \
    if (b != nullptr) {                                                                 \
      b->refs -= 1;                                                                     \
      if (b->refs == 0) {                                                               \
        if (b->on_complete != nullptr) {                                                \
          Block_release(b->on_complete);                                                \
        }                                                                               \
        Name##_clear_cancel(b);                                                         \
        if (b->forward_to != nullptr) {                                                 \
          Name##_box_release(b->forward_to);                                            \
        }                                                                               \
        free(b);                                                                        \
      }                                                                                 \
    }                                                                                   \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_box_release_erased(void* p) {              \
    Name##_box_release((Name##_box*)p);                                                 \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_box_complete(                              \
      Name##_box* first, T val, bool cancelled) {                                       \
    Name##_box* b = Name##_box_retain(first);                                           \
    while (b != nullptr) {                                                              \
      if (b->status != DOLETIS_TASK_PENDING) {                                          \
        Name##_box_release(b);                                                          \
        break;                                                                          \
      }                                                                                 \
      b->status = cancelled ? DOLETIS_TASK_CANCELLED : DOLETIS_TASK_READY;              \
      if (!cancelled) {                                                                 \
        b->value = val;                                                                 \
      }                                                                                 \
      Name##_clear_cancel(b);                                                           \
      void (^cb)(T, bool) = b->on_complete;                                             \
      b->on_complete = nullptr;                                                         \
      Name##_box* fwd = b->forward_to;                                                  \
      b->forward_to = nullptr;                                                          \
      if (cb != nullptr) {                                                              \
        cb(val, cancelled);                                                             \
        Block_release(cb);                                                              \
      }                                                                                 \
      Name##_box_release(b);                                                            \
      b = fwd;                                                                          \
    }                                                                                   \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_box_cancel_erased(void* p) {               \
    Name##_box* b = (Name##_box*)p;                                                     \
    if (b->cancel_ctx != nullptr) {                                                     \
      void* ctx = b->cancel_ctx;                                                        \
      void (*fn)(void*) = b->cancel_fn;                                                 \
      void (*drop)(void*) = b->cancel_drop;                                             \
      b->cancel_ctx = nullptr;                                                          \
      b->cancel_fn = nullptr;                                                           \
      b->cancel_drop = nullptr;                                                         \
      if (fn != nullptr) {                                                              \
        fn(ctx);                                                                        \
      }                                                                                 \
      if (drop != nullptr) {                                                            \
        drop(ctx);                                                                      \
      }                                                                                 \
    }                                                                                   \
    T _zero = {};                                                                       \
    Name##_box_complete(b, _zero, true);                                                \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_pipe_into(Name sub, Name##_box* target) {  \
    if (target->status != DOLETIS_TASK_PENDING) {                                       \
      if (sub.box != nullptr) {                                                         \
        Name##_box_release(sub.box);                                                    \
      }                                                                                 \
      Name##_box_release(target);                                                       \
      return;                                                                           \
    }                                                                                   \
    if (sub.box == nullptr) {                                                           \
      Name##_box_complete(target, sub.immediate, sub.cancelled);                        \
      Name##_box_release(target);                                                       \
      return;                                                                           \
    }                                                                                   \
    if (sub.box->status != DOLETIS_TASK_PENDING) {                                      \
      T v = sub.box->value;                                                             \
      bool c = (sub.box->status == DOLETIS_TASK_CANCELLED);                             \
      Name##_box_release(sub.box);                                                      \
      Name##_box_complete(target, v, c);                                                \
      Name##_box_release(target);                                                       \
      return;                                                                           \
    }                                                                                   \
    Name##_box* dest = target;                                                          \
    if (target->on_complete == nullptr && target->forward_to != nullptr) {              \
      dest = Name##_box_retain(target->forward_to);                                     \
      Name##_box_release(target->forward_to);                                           \
      target->forward_to = nullptr;                                                     \
      Name##_box_release(target);                                                       \
    }                                                                                   \
    Name##_clear_cancel(dest);                                                          \
    dest->cancel_ctx = Name##_box_retain(sub.box);                                      \
    dest->cancel_fn = &Name##_box_cancel_erased;                                        \
    dest->cancel_drop = &Name##_box_release_erased;                                     \
    if (sub.box->on_complete == nullptr && sub.box->forward_to == nullptr) {            \
      sub.box->forward_to = dest;                                                       \
    } else {                                                                            \
      sub.box->on_complete = Block_copy(^ void (T v, bool c) {                          \
        Name##_box_complete(dest, v, c);                                                \
        Name##_box_release(dest);                                                       \
      });                                                                               \
    }                                                                                   \
    Name##_box_release(sub.box);                                                        \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED Name Name##_ready(T v) {                               \
    Name _r = {};                                                                       \
    _r.immediate = v;                                                                   \
    return _r;                                                                          \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED Name Name##_cancelled_task(void) {                     \
    Name _r = {};                                                                       \
    _r.cancelled = true;                                                                \
    return _r;                                                                          \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED Name Name##_from_box(Name##_box* b) {                  \
    Name _r = {};                                                                       \
    _r.box = b;                                                                         \
    return _r;                                                                          \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED Name Name##_pending(Name##_promise* out_promise) {     \
    Name##_box* b = Name##_box_new();                                                   \
    b->refs = 2;                                                                        \
    out_promise->box = b;                                                               \
    return Name##_from_box(b);                                                          \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_resolve(                                   \
      doletis_executor* ex, Name##_promise* p, T val) {                                 \
    Name##_box* b = p->box;                                                             \
    p->box = nullptr;                                                                   \
    if (b == nullptr) {                                                                 \
      return;                                                                           \
    }                                                                                   \
    if (ex != nullptr) {                                                                \
      doletis_executor_post(ex, ^ void (void) {                                         \
        Name##_box_complete(b, val, false);                                             \
        doletis_drain_trampoline();                                                     \
        Name##_box_release(b);                                                          \
      });                                                                               \
    } else {                                                                            \
      Name##_box_complete(b, val, false);                                               \
      doletis_drain_trampoline();                                                       \
      Name##_box_release(b);                                                            \
    }                                                                                   \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_cancel(Name* t) {                          \
    if (t->box != nullptr) {                                                            \
      Name##_box* b = t->box;                                                           \
      t->box = nullptr;                                                                 \
      Name##_box_cancel_erased(b);                                                      \
      Name##_box_release(b);                                                            \
    }                                                                                   \
    t->cancelled = true;                                                                \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED void Name##_promise_drop(Name##_promise* p) {          \
    if (p->box != nullptr) {                                                            \
      Name##_box* b = p->box;                                                           \
      p->box = nullptr;                                                                 \
      if (b->status == DOLETIS_TASK_PENDING) {                                          \
        T _zero = {};                                                                   \
        Name##_box_complete(b, _zero, true);                                            \
      }                                                                                 \
      Name##_box_release(b);                                                            \
    }                                                                                   \
  }                                                                                     \
  static inline DO_LET_IS_UNUSED bool Name##_await(                                     \
      doletis_executor* ex, Name t, T* out) {                                           \
    doletis_drain_trampoline();                                                         \
    if (t.box == nullptr) {                                                             \
      if (!t.cancelled && out != nullptr) {                                             \
        *out = t.immediate;                                                             \
      }                                                                                 \
      return !t.cancelled;                                                              \
    }                                                                                   \
    if (ex != nullptr) {                                                                \
      while (t.box->status == DOLETIS_TASK_PENDING && ex->head != nullptr) {            \
        doletis_executor_run(ex);                                                       \
        doletis_drain_trampoline();                                                     \
      }                                                                                 \
    }                                                                                   \
    doletis_drain_trampoline();                                                         \
    bool ok = (t.box->status == DOLETIS_TASK_READY);                                    \
    if (ok && out != nullptr) {                                                         \
      *out = t.box->value;                                                              \
    }                                                                                   \
    Name##_box_release(t.box);                                                          \
    return ok;                                                                          \
  }                                                                                     \
  DO_LET_IS_OVERLOAD T doletis_elem(Name);                                              \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(Name _t, T v) {   \
    (void)_t;                                                                           \
    return Name##_ready(v);                                                             \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(                  \
      Name _t, Name m) {                                                                \
    (void)_t;                                                                           \
    return m;                                                                           \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_pure(                  \
      doletis_no_monad_t _t, Name m) {                                                  \
    (void)_t;                                                                           \
    return m;                                                                           \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED bool doletis_is_err_impl(           \
      int _tag, Name m) {                                                               \
    (void)_tag;                                                                         \
    if (m.box == nullptr) {                                                             \
      return m.cancelled;                                                               \
    }                                                                                   \
    return m.box->status == DOLETIS_TASK_CANCELLED;                                     \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED Name doletis_bind(                  \
      Name m, Name (^k)(T)) {                                                           \
    if (m.box == nullptr) {                                                             \
      if (m.cancelled) {                                                                \
        return Name##_cancelled_task();                                                 \
      }                                                                                 \
      if (doletis_sync_depth_ < DOLETIS_TRAMPOLINE_LIMIT) {                             \
        doletis_sync_depth_ += 1;                                                       \
        Name r = k(m.immediate);                                                        \
        doletis_sync_depth_ -= 1;                                                       \
        doletis_drain_trampoline();                                                     \
        return r;                                                                       \
      }                                                                                 \
      T v = m.immediate;                                                                \
      doletis_defer_t on_abort = doletis_claim_defer();                                 \
      Name (^k_heap)(T) = Block_copy(k);                                                \
      Name##_box* out_box = Name##_box_new();                                           \
      out_box->refs = 2;                                                                \
      doletis_executor_post(&doletis_trampoline_q_, ^ void (void) {                     \
        if (out_box->status != DOLETIS_TASK_PENDING) {                                  \
          if (on_abort != nullptr) {                                                    \
            on_abort(true);                                                             \
            Block_release(on_abort);                                                    \
          }                                                                             \
          Block_release(k_heap);                                                        \
          Name##_box_release(out_box);                                                  \
          return;                                                                       \
        }                                                                               \
        if (on_abort != nullptr) {                                                      \
          Block_release(on_abort);                                                      \
        }                                                                               \
        Name sub = k_heap(v);                                                           \
        Block_release(k_heap);                                                          \
        Name##_pipe_into(sub, out_box);                                                 \
      });                                                                               \
      return Name##_from_box(out_box);                                                  \
    }                                                                                   \
    if (m.box->status != DOLETIS_TASK_PENDING) {                                        \
      T v = m.box->value;                                                               \
      bool c = (m.box->status == DOLETIS_TASK_CANCELLED);                               \
      Name##_box_release(m.box);                                                        \
      if (c) {                                                                          \
        return Name##_cancelled_task();                                                 \
      }                                                                                 \
      return doletis_bind(Name##_ready(v), k);                                          \
    }                                                                                   \
    doletis_defer_t on_abort = doletis_claim_defer();                                   \
    Name (^k_heap)(T) = Block_copy(k);                                                  \
    Name##_box* out_box = Name##_box_new();                                             \
    out_box->refs = 2;                                                                  \
    out_box->cancel_ctx = Name##_box_retain(m.box);                                     \
    out_box->cancel_fn = &Name##_box_cancel_erased;                                     \
    out_box->cancel_drop = &Name##_box_release_erased;                                  \
    m.box->on_complete = Block_copy(^ void (T v, bool c) {                              \
      if (c || out_box->status == DOLETIS_TASK_CANCELLED) {                             \
        if (on_abort != nullptr) {                                                      \
          on_abort(true);                                                               \
          Block_release(on_abort);                                                      \
        }                                                                               \
        Block_release(k_heap);                                                          \
        T _zero = {};                                                                   \
        Name##_box_complete(out_box, _zero, true);                                      \
        Name##_box_release(out_box);                                                    \
        return;                                                                         \
      }                                                                                 \
      if (on_abort != nullptr) {                                                        \
        Block_release(on_abort);                                                        \
      }                                                                                 \
      Name sub = k_heap(v);                                                             \
      Block_release(k_heap);                                                            \
      Name##_pipe_into(sub, out_box);                                                   \
    });                                                                                 \
    Name##_box_release(m.box);                                                          \
    return Name##_from_box(out_box);                                                    \
  }

#define DOLETIS_DEFINE_TASK_CONV(FromName, FromT, ToName, ToT)                          \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                \
      FromName _t, ToT v) {                                                             \
    (void)_t;                                                                           \
    return ToName##_ready(v);                                                           \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_pure(                \
      FromName _t, ToName m) {                                                          \
    (void)_t;                                                                           \
    return m;                                                                           \
  }                                                                                     \
  DO_LET_IS_OVERLOAD static inline DO_LET_IS_UNUSED ToName doletis_bind(                \
      FromName m, ToName (^k)(FromT)) {                                                 \
    if (m.box == nullptr) {                                                             \
      if (m.cancelled) {                                                                \
        return ToName##_cancelled_task();                                               \
      }                                                                                 \
      if (doletis_sync_depth_ < DOLETIS_TRAMPOLINE_LIMIT) {                             \
        doletis_sync_depth_ += 1;                                                       \
        ToName r = k(m.immediate);                                                      \
        doletis_sync_depth_ -= 1;                                                       \
        doletis_drain_trampoline();                                                     \
        return r;                                                                       \
      }                                                                                 \
      FromT v = m.immediate;                                                            \
      doletis_defer_t on_abort = doletis_claim_defer();                                 \
      ToName (^k_heap)(FromT) = Block_copy(k);                                          \
      ToName##_box* out_box = ToName##_box_new();                                       \
      out_box->refs = 2;                                                                \
      doletis_executor_post(&doletis_trampoline_q_, ^ void (void) {                     \
        if (out_box->status != DOLETIS_TASK_PENDING) {                                  \
          if (on_abort != nullptr) {                                                    \
            on_abort(true);                                                             \
            Block_release(on_abort);                                                    \
          }                                                                             \
          Block_release(k_heap);                                                        \
          ToName##_box_release(out_box);                                                \
          return;                                                                       \
        }                                                                               \
        if (on_abort != nullptr) {                                                      \
          Block_release(on_abort);                                                      \
        }                                                                               \
        ToName sub = k_heap(v);                                                         \
        Block_release(k_heap);                                                          \
        ToName##_pipe_into(sub, out_box);                                               \
      });                                                                               \
      return ToName##_from_box(out_box);                                                \
    }                                                                                   \
    if (m.box->status != DOLETIS_TASK_PENDING) {                                        \
      FromT v = m.box->value;                                                           \
      bool c = (m.box->status == DOLETIS_TASK_CANCELLED);                               \
      FromName##_box_release(m.box);                                                    \
      if (c) {                                                                          \
        return ToName##_cancelled_task();                                               \
      }                                                                                 \
      return doletis_bind(FromName##_ready(v), k);                                      \
    }                                                                                   \
    doletis_defer_t on_abort = doletis_claim_defer();                                   \
    ToName (^k_heap)(FromT) = Block_copy(k);                                            \
    ToName##_box* out_box = ToName##_box_new();                                         \
    out_box->refs = 2;                                                                  \
    out_box->cancel_ctx = FromName##_box_retain(m.box);                                 \
    out_box->cancel_fn = &FromName##_box_cancel_erased;                                 \
    out_box->cancel_drop = &FromName##_box_release_erased;                              \
    m.box->on_complete = Block_copy(^ void (FromT v, bool c) {                          \
      if (c || out_box->status == DOLETIS_TASK_CANCELLED) {                             \
        if (on_abort != nullptr) {                                                      \
          on_abort(true);                                                               \
          Block_release(on_abort);                                                      \
        }                                                                               \
        Block_release(k_heap);                                                          \
        ToT _zero = {};                                                                 \
        ToName##_box_complete(out_box, _zero, true);                                    \
        ToName##_box_release(out_box);                                                  \
        return;                                                                         \
      }                                                                                 \
      if (on_abort != nullptr) {                                                        \
        Block_release(on_abort);                                                        \
      }                                                                                 \
      ToName sub = k_heap(v);                                                           \
      Block_release(k_heap);                                                            \
      ToName##_pipe_into(sub, out_box);                                                 \
    });                                                                                 \
    FromName##_box_release(m.box);                                                      \
    return ToName##_from_box(out_box);                                                  \
  }

#endif
