#include "../mutex/mutex.h";

#ifndef CONDITION_VARIABLE_H
#define CONDITION_VARIABLE_H

namespace dss
{

class condition_variable
{
  condition_variable();
  condition_variable(const condition_variable&) = delete;
  ~condition_variable();

  void notify_one() noexcept;
  void notify_all() noexcept;

  void wait(dss::unique_lock<dss::mutex>& lock);
  template <typename Predicate>
  void wait(dss::unique_lock<dss::mutex>& lock, Predicate stop_waiting);
};

} // namespace dss

#endif // CONDITION_VARIABLE_H
