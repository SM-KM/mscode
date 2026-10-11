#ifndef LATCH_H
#define LATCH_H

#include <atomic>
#include <cstddef>
#include <limits>

namespace dss {
class latch {
 public:
  /// precondition: expected >= 0 and <= max()
  constexpr explicit latch(std::ptrdiff_t expected) : m_counter{expected} {};

  // latches are not copyable nor moveable
  latch(const latch&) = delete;
  ~latch() {};

  /// Decrements the counter by n without blocking if it reaches 0
  /// all the waiting threads are waken up
  void count_down(std::ptrdiff_t n = 1) {
    if (m_counter.fetch_sub(n, std::memory_order_release) == n)
      m_counter.notify_all();
  };
  /// true if the counter is 0, never blocks
  bool try_wait() const noexcept {
    return m_counter.load(std::memory_order_acquire) == 0;
  };

  /// block until the counter is 0, atomic::wait only returns when the
  /// value changed from the one we pass, so we reload and check in a loop
  /// it can also wake up random
  void wait() const {
    for (std::ptrdiff_t c = m_counter.load(std::memory_order_acquire); c != 0;
         c = m_counter.load(std::memory_order_acquire))
      m_counter.wait(c, std::memory_order_acquire);
  };

  /// count_down(n) and then wait
  void arrive_and_wait(std::ptrdiff_t n = 1) {
    count_down(n);
    wait();
  };

 private:
  /// Returns the maximum value of the internal counter supported by the
  /// implementation.
  static constexpr std::ptrdiff_t max() noexcept {
    return std::numeric_limits<std::ptrdiff_t>::max();
  };
  std::atomic<std::ptrdiff_t> m_counter;
};

} // namespace dss

#endif // LATCH_H
