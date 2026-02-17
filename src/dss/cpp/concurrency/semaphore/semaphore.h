#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <cstddef>
#include <semaphore>
namespace dss
{

template <std::ptrdiff_t LeastMaxValue = 10>
class counting_semaphore
{
  constexpr explicit counting_semaphore(std::ptrdiff_t desired);
  counting_semaphore(const counting_semaphore&) = delete;
  ~counting_semaphore();

  void release(std::ptrdiff_t update = 1);
  void acquire();
  [[nodiscard]] bool try_acquire() noexcept;

  template <class Rep, class Period>
  bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time);

  template <class Clock, class Duration>
  [[nodiscard]] bool
  try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time);

  constexpr std::ptrdiff_t max() noexcept;
};

using binary_semaphore = dss::counting_semaphore<1>;

} // namespace dss

#endif // SEMAPHORE_H
