#ifndef BARRIER_H
#define BARRIER_H

#include <algorithm>
#include <atomic>
#include <concepts>
#include <cstddef>

namespace dss {
// barrier is non copiable nor movable

namespace detail {
struct empty_completion {
  void operator()() const noexcept;
};
}; // namespace detail

template <typename CompletionFunction>
  requires std::move_constructible<CompletionFunction> &&
           std::assignable_from<CompletionFunction, CompletionFunction>
class barrier {
 public:
  class arrival_token final {
   public:
    arrival_token(arrival_token&&) = default;
    arrival_token& operator=(arrival_token&&) = default;
    ~arrival_token() = default;

   private:
    friend class barrier;
    explicit arrival_token(std::size_t phase) noexcept : m_phase{phase} {};
    std::size_t m_phase;
  };

  constexpr explicit barrier(std::ptrdiff_t expected,
                             CompletionFunction f = CompletionFunction())
      : m_expected{expected},
        m_remaining{expected},
        m_phase{0},
        m_completion{std::move(f)} {};
  barrier(const barrier&) = delete;
  ~barrier() {};

  // counts n arrivals in the current phase without blocking. if it was
  // the last one the completion function runs here, in this thread
  // precondition: 0 < n <= arrivals missing in this phase
  [[nodiscard]] arrival_token arrive(std::ptrdiff_t n = 1) {
    const std::size_t phase = m_phase.load(std::memory_order_acquire);

    // the thread that gets to zero sees everything the others did
    // before arriving
    if (m_remaining.fetch_sub(n, std::memory_order_acq_rel) == n) {
      m_completion();
      // set up the next phase before releasing anyone
      m_remaining.store(m_expected.load(std::memory_order_relaxed),
                        std::memory_order_relaxed);
      m_phase.fetch_add(1, std::memory_order_release);
      m_phase.notify_all();
    }
    return arrival_token{phase};
  };

  void wait(arrival_token&& arrival) const;

  void arrive_and_wait();
  void arrive_and_drop();

  static constexpr std::ptrdiff_t max() noexcept;

 private:
  std::atomic<std::ptrdiff_t> m_expected;
  std::atomic<std::ptrdiff_t> m_remaining;
  std::atomic<std::size_t> m_phase;
  CompletionFunction m_completion;
};

} // namespace dss

#endif // BARRIER_H
