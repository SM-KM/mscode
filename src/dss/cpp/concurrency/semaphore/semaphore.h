#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <chrono>
#include <cstddef>

#include "../condition_variable/condition_variable.h"
#include "../mutex/mutex.h"

namespace dss {

/// A counter protected by mutex, threads that find it at zero sleep on
/// a condition variable until a release makes it positive
template <std::ptrdiff_t LeastMaxValue = 10>
class counting_semaphore {
  static_assert(LeastMaxValue >= 0, "The maximum value cannot be negative");

 public:
  constexpr explicit counting_semaphore(std::ptrdiff_t desired)
      : m_count{desired} {};
  counting_semaphore(const counting_semaphore&) = delete;
  ~counting_semaphore() {};

  /// Adds update to the counter and wakes up the threads that can proceed
  void release(std::ptrdiff_t update = 1) {
    dss::unique_lock<dss::mutex> lock{m_mutex};
    m_count += update;

    // Notify with the lock held: a thread that acquires and then destroys the
    // semaphore cannot do it until we are completely out of release
    if (update == 1) m_cv.notify_one();
    else if (update > 1)
      m_cv.notify_all();
  }

  void acquire() {
    dss::unique_lock<dss::mutex> lock{m_mutex};
    m_cv.wait(lock, [this] { return m_count > 0; });
    --m_count;
  };
  [[nodiscard]] bool try_acquire() noexcept {
    dss::unique_lock<dss::mutex> lock{m_mutex};
    if (m_count <= 0) return false;
    --m_count;
    return true;
  };

  template <typename Rep, typename Period>
  bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time) {
    dss::unique_lock<dss::mutex> lock{m_mutex};
    if (!m_cv.wait_for(lock, rel_time, [this] { return m_count > 0; }))
      return false;
    --m_count;
    return true;
  };

  template <class Clock, class Duration>
  [[nodiscard]] bool try_acquire_until(
      const std::chrono::time_point<Clock, Duration>& abs_time) {
    dss::unique_lock<dss::mutex> lock{m_mutex};
    if (!m_cv.wait_until(lock, abs_time, [this] { return m_count > 0; }))
      return false;
    --m_count;
    return true;
  };

  constexpr std::ptrdiff_t max() noexcept { return LeastMaxValue; };
  counting_semaphore& operator=(const counting_semaphore&) = delete;

 private:
  dss::mutex m_mutex;
  dss::condition_variable m_cv;
  std::ptrdiff_t m_count;
};

using binary_semaphore = dss::counting_semaphore<1>;

} // namespace dss

#endif // SEMAPHORE_H
