#pragma once

// Object pools

#include <array>
#include <cassert>
#include <concepts>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
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
  struct alignas(T) Slot { std::byte buf[sizeof(T)]; };

  std::array<Slot, N> slots_;
  std::array<std::uint32_t, N> free_{};
  std::size_t free_n_ = N;
  mutable std::mutex m_;
  std::condition_variable cv_;

  std::optional<std::uint32_t> take(bool block) {
    std::lock_guard lk{m_};
    if (block) { cv_.wait(lk, [this] { return free_n_ > 0; }); }
    if (free_n_ == 0) return std::nullopt;
    return free_[--free_n_];
  }

  void give(std::uint32_t idx) {
    { std::lock_guard lk{m_}; free_[free_n_++] = idx; }
    cv_.notify_one();
  }

  template <typename... A> PoolHandle<T, SlotPool> make(bool block, A &&...a) {
    auto idx = take(block);
    if (!idx) return {};
    T *p;
    try {
      p = std::construct_at(reinterpret_cast<T *>(slots_[*idx].buf), std::forward<A>(a)...);
    } catch (...) { give(*idx); throw; }
  };

public:
  using handle = PoolHandle<T, SlotPool>;
  SlotPool() { for(std::uint32_t i = 0; i < N; ++i) free_[i] = i; };
  SlotPool(const SlotPool&) = delete;
  SlotPool& operator=(const SlotPool&) = delete;
  ~SlotPool() { assert(available() == N && "Handles outlived the pool"); }

  /// Non-blocking acquire, if the pool is empty is an empty handle
  template<typename... A> requires std::constructible_from<T, A...>
  handle try_acquire(A&&... a) { return make(false, std::forward<A>(a)...); }
  /// Blocking acquire, wailts until a slot is free
  template<typename... A> requires std::constructible_from<T, A...>
  handle acquire(A&&... a) { return make(true, std::forward<A>(a)...); }

  void release(T* p) {
      std::destroy_at(p);
      give(static_cast<std::uint32_t>(reinterpret_cast<Slot*>(p) - slots_.data()));
  }

  std::size_t available() const { std::lock_guard lk{m_}; return free_n_; };
  static constexpr std::size_t capacity() { return N; }
  // clang-format on
};
