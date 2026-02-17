#ifndef BARRIER_H
#define BARRIER_H

#include <barrier>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace dss
{
// barrier is non copiable nor movable
template <typename CompletionFunction>
  requires std::move_constructible<CompletionFunction> &&
           std::assignable_from<CompletionFunction, CompletionFunction>
class barrier
{
public:
  class arrival_token final
  {
  public:
    arrival_token(arrival_token&&) = default;
    arrival_token& operator=(arrival_token&&) = default;
    ~arrival_token() = default;

  private:
    friend class barrier;
  };
  constexpr explicit barrier(std::ptrdiff_t expected,
                             CompletionFunction f = CompletionFunction());
  barrier(const barrier&) = delete;
  ~barrier();

  [[nodiscard]] arrival_token arrive(std::ptrdiff_t n = 1);
  void wait(arrival_token&& arrival) const;

  void arrive_and_wait();
  void arrive_and_drop();

  static constexpr std::ptrdiff_t max() noexcept;
};

} // namespace dss

#endif // BARRIER_H
