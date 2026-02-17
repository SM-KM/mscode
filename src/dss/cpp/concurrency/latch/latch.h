#ifndef LATCH_H
#define LATCH_H

#include <cstddef>
namespace dss
{
class latch
{
public:
  constexpr explicit latch(std::ptrdiff_t expected);

  // latches are not copyable nor moveable
  latch(const latch&) = delete;
  ~latch();

  void count_down(std::ptrdiff_t n = 1);
  bool try_wait() const noexcept;
  void wait() const;
  void arrive_and_wait(std::ptrdiff_t n = 1);

private:
  // Returns the maximum value of the internal counter supported by the
  // implementation.
  static constexpr std::ptrdiff_t max() noexcept;
};

} // namespace dss

#endif // LATCH_H
