#ifndef SHARED_MUTEX_H
#define SHARED_MUTEX_H

#include "mutex.h"
#include <chrono>
#include <mutex>
namespace dss
{
class shared_mutex
{
  shared_mutex();
  shared_mutex(const shared_mutex&) = delete;
  ~shared_mutex();

  void lock();
  void unlock();
  [[nodiscard]] bool try_lock();

  void lock_shared();
  void unlock_shared();
  [[nodiscard]] bool try_lock_shared();
};

template <typename Mutex>
class shared_lock
{
public:
  using mutex_type = Mutex;

  shared_lock() noexcept;
  shared_lock(shared_lock&& other) noexcept;
  explicit shared_lock(mutex_type& m);
  shared_lock(mutex_type& m, std::defer_lock_t t) noexcept;
  shared_lock(mutex_type& m, std::try_to_lock_t t);
  shared_lock(mutex_type& m, std::adopt_lock_t t);

  template <class Rep, class Period>
  shared_lock(mutex_type& m,
              const std::chrono::duration<Rep, Period>& timeout_duration);

  template <class Clock, class Duration>
  shared_lock(mutex_type& m,
              const std::chrono::time_point<Clock, Duration>& timeout_duration);

  shared_lock& operator=(shared_lock&& other) noexcept;
  void lock();
  void unlock();

  [[nodiscard]] bool try_lock();

  template <class Rep, class Period>
  bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration);
  template <class Clock, class Duration>
  [[nodiscard]] bool
  try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time);

  template <typename M>
  void swap(shared_lock<dss::mutex>& other) noexcept;

  mutex_type *release() noexcept;
  mutex_type *mutex() const noexcept;

  [[nodiscard]] bool owns_lock() const noexcept;
  explicit operator bool() const noexcept;
};

} // namespace dss

#endif // SHARED_MUTEX_H
