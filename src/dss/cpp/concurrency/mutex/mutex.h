#ifndef MUTEX_H
#define MUTEX_H

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
};

class scoped_lock
{
};

class lock
{
};

} // namespace dss

#endif // MUTEX_H
