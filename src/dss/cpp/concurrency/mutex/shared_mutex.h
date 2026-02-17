#ifndef SHARED_MUTEX_H
#define SHARED_MUTEX_H

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

class shared_lock
{
};
} // namespace dss

#endif // SHARED_MUTEX_H
