#ifndef MUTEX_H
#define MUTEX_H

#include <chrono>
#include <mutex>
namespace dss
{

class mutex
{
public:
  constexpr mutex() noexcept;
  mutex(const mutex&) = delete; // mutexes are not copyable
  ~mutex();

  void lock();
  void unlock();
  [[nodiscard]] bool try_lock();
};

class timed_mutex
{
};

class lock_guard
{
};

template <typename Mutex>
class unique_lock
{
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
  [[nodiscard]] bool
  try_lock_for(std::chrono::duration<Rep, Period>& timeout_duration);

  template <typename Clock, typename Duration>
  [[nodiscard]] bool
  try_lock_until(std::chrono::duration<Clock, Duration>& timeout_duration);
};

class scoped_lock
{
};

class lock
{
};

} // namespace dss

#endif // MUTEX_H
