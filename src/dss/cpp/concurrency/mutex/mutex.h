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

  [[nodiscard]] bool try_lock();

  template <typename Rep, typename Period>
  [[nodiscard]] bool try_lock_for(
      const std::chrono::duration<Rep, Period>& timeout_duration);

  template <typename Clock, typename Duration>
  [[nodiscard]] bool try_lock_for(
      const std::chrono::time_point<Clock, Duration>& timeout_time);

  void unlock();

 private:
  mutex m_mutex;
  std::condition_variable_any cv;
  bool m_locked{false};
};

template <typename Mutex>
class lock_guard {
 public:
  using mutex_type = Mutex;

  explicit lock_guard(mutex_type& m);
  lock_guard(mutex_type& m, std::adopt_lock_t t);
  lock_guard(const lock_guard&) = delete;
};

template <typename Mutex>
class unique_lock {
  using mutex_type = Mutex;

 public:
  unique_lock() noexcept;
  unique_lock(unique_lock&& lock) noexcept;
  explicit unique_lock(mutex_type& m);

  unique_lock(mutex_type& m, std::defer_lock_t t) noexcept;
  unique_lock(mutex_type& m, std::try_to_lock_t t);
  unique_lock(mutex_type& m, std::adopt_lock_t t);

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
      std::chrono::duration<Clock, Duration>& timeout_duration);

  void unlock();

  void swap(unique_lock& other) noexcept;
  mutex_type* release() noexcept;
  mutex_type* mutex() const noexcept;

  [[nodiscard]] bool owns_lock() const noexcept;
  explicit operator bool() const noexcept;
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
