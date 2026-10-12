#ifndef CONDITION_VARIABLE_H
#define CONDITION_VARIABLE_H

#include <pthread.h>

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <system_error>

#include "../mutex/mutex.h"

namespace dss {

enum class cv_status { no_timeout, timeout };

class condition_variable {
 public:
  condition_variable() {};
  condition_variable(const condition_variable&) = delete;
  ~condition_variable() { pthread_cond_destroy(&m_cond); };

  /// wakes up one of the waiting threads if any
  void notify_one() noexcept { pthread_cond_signal(&m_cond); };
  /// wakes up all the waiting threads
  void notify_all() noexcept { pthread_cond_broadcast(&m_cond); };

  /// atomically unlocks the mutex and blocks, when it returns the mutex
  /// is locked again.
  void wait(dss::unique_lock<dss::mutex>& lock) {
    check_lock(lock);
    int e = pthread_cond_wait(&m_cond, lock.mutex()->native_handle());
    if (e != 0) throw std::system_error(e, std::generic_category());
  };

  /// protects against spurious wakeups and lost notifications
  template <typename Predicate>
  void wait(dss::unique_lock<dss::mutex>& lock, Predicate stop_waiting) {
    while (!stop_waiting()) wait(lock);
  };

  /// relative timeouts are turned into and absolute deadline on the steady
  /// clock, clamped so a huge duration cannot overflow
  template <typename Rep, typename Period>
  dss::cv_status wait_for(dss::unique_lock<dss::mutex>& lock,
                          const std::chrono::duration<Rep, Period> rel_time) {
    return wait_until(lock, deadline_after(rel_time));
  };

  template <typename Rep, typename Period, typename Predicate>
  [[nodiscard]] bool wait_for(dss::unique_lock<dss::mutex>& lock,
                              const std::chrono::duration<Rep, Period> rel_time,
                              Predicate stop_waiting) {
    return wait_until(lock, deadline_after(rel_time), std::move(stop_waiting));
  };

  template <typename Clock, typename Duration>
  dss::cv_status wait_until(
      dss::unique_lock<dss::mutex>& lock,
      const std::chrono::time_point<Clock, Duration> rel_time) {
    check_lock(lock);
    using sys = std::chrono::system_clock;
    const auto clock_now = Clock::now();
    const auto sys_now = sys::now();
    sys::time_point sys_deadline;

    if (rel_time <= clock_now) {
      sys_deadline = sys_now;
    } else {
      /// compare as long double first, converting an enormous duration to the
      /// system clock one is what dould overflow
      using ld = std::chrono::duration<long double>;
      if (ld(rel_time - clock_now) >= ld(sys::time_point::max() - sys_now))
        sys_deadline = sys::time_point::max();
      else
        sys_deadline =
            sys_now + std::chrono::ceil<sys::duration>(rel_time - clock_now);
    }

    timed_wait(lock, sys_deadline);
    return Clock::now() < rel_time ? dss::cv_status::no_timeout
                                   : dss::cv_status::timeout;
  };

  template <typename Clock, typename Duration, typename Predicate>
  [[nodiscard]] bool wait_until(
      dss::unique_lock<dss::mutex>& lock,
      const std::chrono::time_point<Clock, Duration> rel_time,
      Predicate stop_waiting);

  condition_variable& operator=(const condition_variable&) = delete;

 private:
  // acording to the standard it says that the caller must own the lock,
  // handling pthread an unlocked mutex
  static void check_lock(const dss::unique_lock<dss::mutex>& lock) {
    if (!lock.owns_lock() || lock.mutex() == nullptr)
      throw std::system_error(
          std::make_error_code(std::errc::operation_not_permitted));
  }

  template <typename Rep, typename Period>
  static std::chrono::steady_clock::time_point deadline_after(
      const std::chrono::duration<Rep, Period>& rel_time) {
    using clock = std::chrono::steady_clock;
    const auto now = clock::now();
    using ld = std::chrono::duration<long double>;
    if (ld(rel_time) >= ld(clock::time_point::max() - now))
      return clock::time_point::max();
    return now + std::chrono::ceil<clock::duration>(rel_time);
  }

  void timed_wait(dss::unique_lock<dss::mutex>& lock,
                  std::chrono::system_clock::time_point deadline) {
    using namespace std::chrono;
    auto ns = duration_cast<nanoseconds>(deadline.time_since_epoch());
    auto secs = duration_cast<seconds>(ns);
    timespec ts;
    ts.tv_sec = static_cast<time_t>(secs.count());
    ts.tv_nsec = static_cast<long>((ns - secs).count());

    int e = pthread_cond_timedwait(&m_cond, lock.mutex()->native_handle(), &ts);
    if (e != 0 && e != ETIMEDOUT)
      throw std::system_error(e, std::generic_category());
  }

  pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;
};

} // namespace dss

#endif // CONDITION_VARIABLE_H
