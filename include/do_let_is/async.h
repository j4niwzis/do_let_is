#ifndef DO_LET_IS_ASYNC_H
#define DO_LET_IS_ASYNC_H

#ifndef __cplusplus
#include "do_let_is/c/async.h"
#else

#include <cstddef>
#include <deque>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

#include "do_let_is/do_let_is.h"

namespace doletis {
namespace internal {

template <typename Sig>
class move_fn;

template <typename R, typename... Args>
class move_fn<R(Args...)> {
  struct base {
    virtual ~base() = default;
    virtual R call(Args... args) = 0;
  };
  template <typename F>
  struct holder : base {
    F fn;
    explicit holder(F f) : fn(std::move(f)) {}
    R call(Args... args) override { return fn(std::forward<Args>(args)...); }
  };
  std::unique_ptr<base> ptr_;

 public:
  move_fn() = default;
  move_fn(std::nullptr_t) noexcept {}  // NOLINT
  template <typename F>
    requires(!std::is_same_v<std::decay_t<F>, move_fn>)
  move_fn(F&& f) : ptr_(std::make_unique<holder<std::decay_t<F>>>(std::forward<F>(f))) {}  // NOLINT

  move_fn(move_fn&&) noexcept = default;
  move_fn& operator=(move_fn&&) noexcept = default;
  move_fn& operator=(std::nullptr_t) noexcept {
    ptr_.reset();
    return *this;
  }
  move_fn(const move_fn&) = delete;
  move_fn& operator=(const move_fn&) = delete;

  explicit operator bool() const noexcept { return static_cast<bool>(ptr_); }
  R operator()(Args... args) { return ptr_->call(std::forward<Args>(args)...); }
};

}  // namespace internal

enum class task_state {
  pending,
  ready,
  cancelled,
};

class executor {
 public:
  void post(internal::move_fn<void()> fn) { queue_.push_back(std::move(fn)); }

  std::size_t run() {
    std::size_t count = 0;
    while (!queue_.empty()) {
      auto fn = std::move(queue_.front());
      queue_.pop_front();
      fn();
      ++count;
    }
    return count;
  }

  bool empty() const noexcept { return queue_.empty(); }

 private:
  std::deque<internal::move_fn<void()>> queue_;
};

namespace internal {

inline constexpr unsigned kTrampolineLimit = 32;

inline unsigned& sync_depth() {
  thread_local unsigned depth = 0;
  return depth;
}

inline bool& in_trampoline() {
  thread_local bool active = false;
  return active;
}

inline executor& trampoline_queue() {
  thread_local executor q;
  return q;
}

inline void drain_trampoline() {
  if (sync_depth() == 0 && !in_trampoline() && !trampoline_queue().empty()) {
    in_trampoline() = true;
    trampoline_queue().run();
    in_trampoline() = false;
  }
}

template <typename T>
struct task_box : std::enable_shared_from_this<task_box<T>> {
  task_state status = task_state::pending;
  std::optional<T> value;
  move_fn<void(T*, bool)> on_complete;
  std::shared_ptr<task_box<T>> forward_to;
  move_fn<void()> cancel_upstream;
  std::shared_ptr<void> keep_alive;

  static void complete(std::shared_ptr<task_box<T>> first, std::optional<T> val, bool is_cancelled) {
    auto b = std::move(first);
    while (b && b->status == task_state::pending) {
      b->status = is_cancelled ? task_state::cancelled : task_state::ready;
      b->cancel_upstream = nullptr;
      auto cb = std::move(b->on_complete);
      auto fwd = std::move(b->forward_to);
      if (!is_cancelled) {
        b->value = (!cb && !fwd) ? std::move(val) : val;
      }
      if (cb) {
        cb(b->value ? &*b->value : nullptr, is_cancelled);
      }
      b->keep_alive.reset();
      b = std::move(fwd);
    }
  }

  void cancel() {
    if (auto up = std::move(cancel_upstream)) {
      up();
    }
    complete(this->shared_from_this(), std::nullopt, true);
  }
};

}  // namespace internal

template <typename T>
class task;

template <typename T>
class promise {
 public:
  promise() = default;
  explicit promise(std::shared_ptr<internal::task_box<T>> box) : box_(std::move(box)) {}

  promise(promise&&) noexcept = default;
  promise& operator=(promise&&) noexcept = default;
  promise(const promise&) = delete;
  promise& operator=(const promise&) = delete;

  ~promise() {
    if (box_ && box_->status == task_state::pending) {
      internal::task_box<T>::complete(std::move(box_), std::nullopt, true);
    }
  }

  void resolve(T val, executor* ex = nullptr) {
    auto b = std::move(box_);
    if (!b) {
      return;
    }
    if (ex != nullptr) {
      ex->post([b = std::move(b), v = std::move(val)]() mutable {
        internal::task_box<T>::complete(std::move(b), std::move(v), false);
        internal::drain_trampoline();
      });
    } else {
      internal::task_box<T>::complete(std::move(b), std::move(val), false);
      internal::drain_trampoline();
    }
  }

 private:
  std::shared_ptr<internal::task_box<T>> box_;
};

template <typename T>
class task {
 public:
  using value_type = T;

  static task ready(T val) {
    task t;
    t.immediate_ = std::move(val);
    return t;
  }

  static task cancelled() {
    task t;
    t.cancelled_ = true;
    return t;
  }

  static std::pair<task<T>, promise<T>> pending() {
    auto b = std::make_shared<internal::task_box<T>>();
    task<T> t;
    t.box_ = b;
    return {std::move(t), promise<T>{std::move(b)}};
  }

  bool has_error() const noexcept {
    if (!box_) {
      return cancelled_;
    }
    return box_->status == task_state::cancelled;
  }

  void cancel() {
    if (box_) {
      auto b = std::move(box_);
      b->cancel();
    }
    cancelled_ = true;
  }

  std::optional<T> await(executor* ex = nullptr) && {
    internal::drain_trampoline();
    if (!box_) {
      return cancelled_ ? std::nullopt : std::move(immediate_);
    }
    if (ex != nullptr) {
      while (box_->status == task_state::pending && !ex->empty()) {
        ex->run();
        internal::drain_trampoline();
      }
    }
    internal::drain_trampoline();
    if (box_->status == task_state::ready) {
      return std::move(box_->value);
    }
    return std::nullopt;
  }

  std::shared_ptr<internal::task_box<T>>& box_ref() noexcept { return box_; }
  std::optional<T>& immediate_ref() noexcept { return immediate_; }
  bool is_cancelled_immediate() const noexcept { return cancelled_; }

 private:
  std::shared_ptr<internal::task_box<T>> box_;
  std::optional<T> immediate_;
  bool cancelled_ = false;
};

template <typename T>
task<std::decay_t<T>> ready(T&& val) {
  return task<std::decay_t<T>>::ready(std::forward<T>(val));
}

template <typename T>
struct is_task : std::false_type {};

template <typename T>
struct is_task<task<T>> : std::true_type {};

template <typename T, typename V>
auto doletis_pure(monad_tag<task<T>>, V&& val) {
  if constexpr (is_task<std::decay_t<V>>::value) {
    return std::forward<V>(val);
  } else {
    return ready(std::forward<V>(val));
  }
}

namespace internal {

template <typename R>
void pipe_task_into(task<R>&& sub, std::shared_ptr<task_box<R>> target) {
  if (!target || target->status != task_state::pending) {
    return;
  }
  if (!sub.box_ref()) {
    if (sub.is_cancelled_immediate()) {
      task_box<R>::complete(std::move(target), std::nullopt, true);
    } else {
      task_box<R>::complete(std::move(target), std::move(sub.immediate_ref()), false);
    }
    return;
  }
  auto sb = std::move(sub.box_ref());
  if (sb->status != task_state::pending) {
    bool c = (sb->status == task_state::cancelled);
    task_box<R>::complete(std::move(target), std::move(sb->value), c);
    return;
  }
  auto dest = std::move(target);
  if (!dest->on_complete && dest->forward_to) {
    dest = std::move(dest->forward_to);
  }
  dest->cancel_upstream = [sb]() { sb->cancel(); };
  if (!sb->on_complete && !sb->forward_to) {
    sb->forward_to = std::move(dest);
  } else {
    sb->on_complete = [dest = std::move(dest)](R* v, bool c) mutable {
      task_box<R>::complete(std::move(dest), (v != nullptr && !c) ? std::optional<R>{*v} : std::nullopt, c);
    };
  }
}

}  // namespace internal

template <typename T, typename Defer, typename F>
auto doletis_bind_defer(task<T>&& m, const Defer& defer, F&& f) {
  using RTask = std::decay_t<std::invoke_result_t<F, T&>>;
  using R = typename RTask::value_type;

  if (!m.box_ref()) {
    if (m.is_cancelled_immediate()) {
      defer(true);
      return RTask::cancelled();
    }
    if (internal::sync_depth() < internal::kTrampolineLimit) {
      ++internal::sync_depth();
      auto slot = std::make_shared<T>(std::move(*m.immediate_ref()));
      auto res = std::forward<F>(f)(*slot);
      if (res.box_ref() && res.box_ref()->status == task_state::pending) {
        res.box_ref()->keep_alive = std::move(slot);
      }
      --internal::sync_depth();
      internal::drain_trampoline();
      return res;
    }
    auto slot = std::make_shared<T>(std::move(*m.immediate_ref()));
    auto out_box = std::make_shared<internal::task_box<R>>();
    internal::trampoline_queue().post(
        [out_box, slot = std::move(slot), fn = std::forward<F>(f), defer = defer]() mutable {
          if (out_box->status != task_state::pending) {
            defer(true);
            return;
          }
          auto sub = std::move(fn)(*slot);
          if (sub.box_ref() && sub.box_ref()->status == task_state::pending) {
            sub.box_ref()->keep_alive = std::move(slot);
          }
          internal::pipe_task_into<R>(std::move(sub), std::move(out_box));
        });
    RTask out;
    out.box_ref() = std::move(out_box);
    return out;
  }

  auto in_box = std::move(m.box_ref());
  if (in_box->status != task_state::pending) {
    if (in_box->status == task_state::cancelled) {
      defer(true);
      return RTask::cancelled();
    }
    return doletis_bind_defer(task<T>::ready(std::move(*in_box->value)), defer, std::forward<F>(f));
  }

  auto out_box = std::make_shared<internal::task_box<R>>();
  out_box->cancel_upstream = [in_box]() { in_box->cancel(); };
  in_box->on_complete = [in_box, out_box, fn = std::forward<F>(f), defer = defer](
                            T* val_ptr, bool is_cancelled) mutable {
    if (is_cancelled || val_ptr == nullptr || out_box->status == task_state::cancelled) {
      defer(true);
      internal::task_box<R>::complete(std::move(out_box), std::nullopt, true);
      return;
    }
    auto sub = std::move(fn)(*val_ptr);
    if (sub.box_ref() && sub.box_ref()->status == task_state::pending) {
      sub.box_ref()->keep_alive = in_box;
    }
    internal::pipe_task_into<R>(std::move(sub), std::move(out_box));
  };
  RTask out;
  out.box_ref() = std::move(out_box);
  return out;
}

template <typename T, typename Defer, typename F>
auto doletis_bind_defer(task<T>& m, const Defer& defer, F&& f) {
  return doletis_bind_defer(std::move(m), defer, std::forward<F>(f));
}

template <typename T, typename F>
auto doletis_bind(task<T>&& m, F&& f) {
  return doletis_bind_defer(std::move(m), internal::noop_defer{}, std::forward<F>(f));
}

template <typename T, typename F>
auto doletis_bind(task<T>& m, F&& f) {
  return doletis_bind_defer(std::move(m), internal::noop_defer{}, std::forward<F>(f));
}

}  // namespace doletis

#endif  // __cplusplus

#endif  // DO_LET_IS_ASYNC_H
