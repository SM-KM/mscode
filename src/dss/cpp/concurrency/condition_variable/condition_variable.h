#ifndef CONDITION_VARIABLE_H
#define CONDITION_VARIABLE_H

#include "../mutex/mutex.h"
#include <chrono>

namespace dss
{

enum class cv_status
{
  no_timeout,
  timeout
};

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

  template <typename Rep, typename Period>
  dss::cv_status wait_for(dss::unique_lock<dss::mutex>& lock,
                          const std::chrono::duration<Rep, Period> rel_time);

  template <typename Rep, typename Period, typename Predicate>
  [[nodiscard]] bool wait_for(dss::unique_lock<dss::mutex>& lock,
                              const std::chrono::duration<Rep, Period> rel_time,
                              Predicate stop_waiting);

  template <typename Clock, typename Duration>
  dss::cv_status
  wait_until(dss::unique_lock<dss::mutex>& lock,
             const std::chrono::time_point<Clock, Duration> rel_time);

  template <typename Clock, typename Duration, typename Predicate>
  [[nodiscard]] bool
  wait_until(dss::unique_lock<dss::mutex>& lock,
             const std::chrono::time_point<Clock, Duration> rel_time,
             Predicate stop_waiting);
};

} // namespace dss

#endif // CONDITION_VARIABLE_H
