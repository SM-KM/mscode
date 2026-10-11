#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <system_error>
#include <utility>

namespace dss {

template <typename... Lockables>
void lock(Lockables&... lockables);

namespace detail {
template <typename Tuple, typename F, std::size_t... Is>
void for_index(Tuple& t, std::size_t idx, F&& f, std::index_sequence<Is...>) {
  (void)((idx == Is ? (f(std::get<Is>(t)), true) : false) || ...);
};
}; // namespace detail

class mutex {
 public:
  constexpr mutex() noexcept {};
  mutex(const mutex&) = delete;
  ~mutex() { pthread_mutex_destroy(&m_handle); };

  void lock() {
    int e = pthread_mutex_lock(&m_handle);
    if (e != 0) throw std::system_error(e, std::generic_category());
  };
  void unlock() { pthread_mutex_unlock(&m_handle); };
  [[nodiscard]] bool try_lock() {
    return pthread_mutex_trylock(&m_handle) == 0;
  };

  mutex& operator=(const mutex&) = delete;

 private:
  pthread_mutex_t m_handle = PTHREAD_MUTEX_INITIALIZER;
};

class timed_mutex {
 public:
  timed_mutex() = default;
  timed_mutex(const timed_mutex&) = delete;
  ~timed_mutex() {};

  void lock() {
    m_mutex.lock();
    try {
      while (m_locked) cv.wait(m_mutex);
    } catch (...) {
      // WARN: specify the catch, and use an specific throw
      // and dont let only this ambig...
      m_mutex.unlock();
      throw;
    }

    m_locked = true;
    m_mutex.unlock();
  }

  [[nodiscard]] bool try_lock() {
    m_mutex.lock();
    bool got = !m_locked;
    if (got) m_locked = true;
    m_mutex.unlock();
    return got;
  }

  template <typename Rep, typename Period>
  [[nodiscard]] bool try_lock_for(
      const std::chrono::duration<Rep, Period>& timeout_duration) {
    // a relative time is converted to an absolute time on a steady clock
    using clock = std::chrono::steady_clock;
    auto now = clock::now();

    // comparing the long double first, converting a huge duration to the
    // clock duration is the thing that would overflow
    using ld = std::chrono::duration<long double>;
    if (ld(timeout_duration) >= ld(clock::time_point::max() - now))
      return try_lock_until(clock::time_point::max());
    return try_lock_until(now +
                          std::chrono::ceil<clock::duration>(timeout_duration));
  }

  template <typename Clock, typename Duration>
  [[nodiscard]] bool try_lock_until(
      const std::chrono::time_point<Clock, Duration>& timeout_duration) {
    m_mutex.lock();
    bool got = false;
    try {
      got = cv.wait_until(m_mutex, timeout_duration,
                          [this] { return !m_locked; });
    } catch (...) {
      m_mutex.unlock();
      throw;
    }

    if (got) m_locked = true;
    m_mutex.unlock();
    return got;
  }

  template <typename Clock, typename Duration>
  [[nodiscard]] bool try_lock_for(
      const std::chrono::time_point<Clock, Duration>& timeout_time);

  void unlock() {
    m_mutex.lock();
    m_locked = false;

    // notify while we still hold the mtx, so that a thread sees the
    // lock free and destroys the timed_mutex cannot kill the cv
    cv.notify_one();
    m_mutex.unlock();
  }

  timed_mutex& operator=(const timed_mutex&) = delete;

 private:
  mutex m_mutex;
  std::condition_variable_any cv;
  bool m_locked{false};
};

template <typename Mutex>
class lock_guard {
 public:
  using mutex_type = Mutex;

  explicit lock_guard(mutex_type& m) : m_mutex{m} { m_mutex.lock(); };

  lock_guard(mutex_type& m, [[maybe_unused]] std::adopt_lock_t t)
      : m_mutex{m} {};
  lock_guard(const lock_guard&) = delete;

  ~lock_guard() { m_mutex.unlock(); };
  lock_guard& operator=(const lock_guard&) = delete;

 private:
  mutex_type& m_mutex;
};

template <typename Mutex>
class unique_lock {
  using mutex_type = Mutex;

 public:
  unique_lock() noexcept = default;
  unique_lock(unique_lock&& lock) noexcept
      : m_mutex{lock}, m_owns{lock.m_owns} {
    lock.m_mutex = nullptr;
    lock.m_owns = false;
  };
  explicit unique_lock(mutex_type& m) : m_mutex{m} {
    m_mutex->lock();
    m_owns = true;
  };

  unique_lock(mutex_type& m, [[maybe_unused]] std::defer_lock_t t) noexcept
      : m_mutex{&m} {};
  unique_lock(mutex_type& m, [[maybe_unused]] std::try_to_lock_t t)
      : m_mutex{&m}, m_owns{m.try_lock()} {};
  unique_lock(mutex_type& m, [[maybe_unused]] std::adopt_lock_t t)
      : m_mutex{&m}, m_owns{true} {};

  template <typename Rep, typename Period>
  unique_lock(mutex_type& m,
              const std::chrono::duration<Rep, Period>& timout_duration);
  template <class Clock, class Duration>
  unique_lock(mutex_type& m,
              const std::chrono::time_point<Clock, Duration>& timeout_time);

  ~unique_lock();
  unique_lock& operator=(unique_lock&& other);

  // TODO: chrono support with concepts

  // Locking strategies
  void Lock();
  [[nodiscard]] bool try_lock();

  template <typename Rep, typename Period>
  [[nodiscard]] bool try_lock_for(
      std::chrono::duration<Rep, Period>& timeout_duration);

  template <typename Clock, typename Duration>
  [[nodiscard]] bool try_lock_until(
      std::chrono::time_point<Clock, Duration>& timeout_duration);

  void unlock();

  void swap(unique_lock& other) noexcept;
  mutex_type* release() noexcept;
  mutex_type* mutex() const noexcept;

  [[nodiscard]] bool owns_lock() const noexcept;
  explicit operator bool() const noexcept;

 private:
  void check_can_lock() const {
    if (m_mutex == nullptr)
      throw std::system_error(
          std::make_error_code(std::errc::operation_not_permitted));
    if (m_owns)
      throw std::system_error(
          std::make_error_code(std::errc::resource_deadlock_would_occur));
  }

  mutex_type* m_mutex{nullptr};
  bool m_owns{false};
};

template <typename... MutexTypes>
class scoped_lock {
  explicit scoped_lock(MutexTypes&... m);
  scoped_lock(std::adopt_lock_t t, MutexTypes&... m);
  scoped_lock(const scoped_lock&) = delete;
  ~scoped_lock();
};

template <typename... Lockables>
void lock(Lockables&... lockables) {};

} // namespace dss

#endif // MUTEX_H
