#ifndef MUTEX_H
#define MUTEX_H

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

class unique_lock
{
};

class scoped_lock
{
};

class lock
{
};

} // namespace dss

#endif // MUTEX_H
