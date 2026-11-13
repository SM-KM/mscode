#pragma once

// Object pools

#include <concepts>
#include <utility>

template <typename P, typename T>
concept PoolOf = requires(P &p, T *t) { p.release(t); };

template <typename T>
concept Resettable = requires(T &t) { t.reset(); };

// RAII handle, move only pool
template <typename T, typename Pool> class PoolHandle {
  Pool *pool_ = nullptr;
  T *p_ = nullptr;

public:
  PoolHandle() = default;
  PoolHandle(Pool *pool, T *p) : pool_{pool}, p_{p} {
    static_assert(PoolOf<Pool, T>, "Pool must provide release(T*)");
  }
  PoolHandle(const PoolHandle &) = delete;
  PoolHandle &operator=(const PoolHandle &) = delete;
  // clang-format off
  PoolHandle(PoolHandle &&o) noexcept
      : pool_{std::exchange(o.pool_, nullptr)}, p_{std::exchange(o.p_, nullptr)} {};

  PoolHandle &operator=(PoolHandle &&o) noexcept {
      if(this != &o) { reset(); pool_ = std::exchange(o.pool_, nullptr); p_ = std::exchange(o.p_, nullptr);
	  return *this;
      } 
  };
  ~PoolHandle() { reset(); }
  void reset() { if(p_) { pool_->release(); p_ = nullptr; pool_ = nullptr; } }
  // clang-format on

  T *operator->() const { return p_; }
  T &operator*() const { return *p_; }
  T get() const { return p_; }
  explicit operator bool() const { return p_ != nullptr; }
};

// Slot pool: fixed N slots of raw storage, lifetime per use
// clang-format off
template <typename T, std::size_t N> requires(N > 0) && std::destructible<T>
class SlotPool {
  // clang-format on
};
