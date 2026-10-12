#ifndef JTHREAD_H
#define JTHREAD_H

#include <algorithm>
#include <atomic>
#include <functional>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
namespace dss {

namespace detail {
struct stop_state {
  std::atomic<bool> stopped{false};
  std::atomic<std::size_t> sources{0};
};
}; // namespace detail

class stop_source;
class stop_token {
 public:
  stop_token() noexcept = default;

  [[nodiscard]] bool stop_requested() const noexcept {
    return m_state && m_state->stopped.load(std::memory_order_acquire);
  };

  [[nodiscard]] bool stop_possible() const noexcept {
    return m_state && (m_state->stopped.load(std::memory_order_acquire) ||
                       m_state->sources.load(std::memory_order_acquire) > 0);
  };

  void swap(stop_token& other) noexcept { m_state.swap(other.m_state); };

 private:
  friend class stop_source;
  explicit stop_token(std::shared_ptr<detail::stop_state> s) noexcept
      : m_state{std::move(s)} {}
  std::shared_ptr<detail::stop_state> m_state;
};

struct nonstopstate_t {
  explicit nonstopstate_t() = default;
};

class stop_source {
 public:
  stop_source() : m_state{std::make_shared<detail::stop_state>()} {
    m_state->sources.store(1, std::memory_order_relaxed);
  };

  explicit stop_source([[maybe_unused]] dss::nonstopstate_t nss) noexcept {};
  stop_source(const stop_source& other) noexcept : m_state(other.m_state) {
    if (m_state) m_state->sources.fetch_add(1, std::memory_order_relaxed);
  }

  stop_source(stop_source&& other) noexcept
      : m_state(std::move(other.m_state)) {}

  ~stop_source() {
    if (m_state) m_state->sources.fetch_sub(1, std::memory_order_acq_rel);
  }

  [[nodiscard]] bool request_stop() noexcept {
    if (!m_state) return false;
    return !m_state->stopped.exchange(true, std::memory_order_acq_rel);
  }

  void swap(dss::stop_source& other) noexcept { m_state.swap(other.m_state); }
  dss::stop_source& operator=(const dss::stop_source& other) noexcept {
    stop_source tmp(other);
    swap(tmp);
    return *this;
  }
  dss::stop_source& operator=(dss::stop_source&& other) noexcept {
    stop_source tmp(std::move(other));
    swap(tmp);
    return *this;
  }
  [[nodiscard]] bool stop_requested() const noexcept {
    return m_state && m_state->stopped.load(std::memory_order_acquire);
  }
  [[nodiscard]] bool stop_possible() const noexcept {
    return m_state != nullptr;
  }

  [[nodiscard]] stop_token get_token() const noexcept {
    return stop_token(m_state);
  }
  friend bool operator==(const stop_source& a, const stop_source& b) noexcept {
    return a.m_state == b.m_state;
  }

 private:
  std::shared_ptr<detail::stop_state> m_state;
};

class jthread {
 public:
  using id = std::thread::id;

  jthread() noexcept : m_stop{dss::nonstopstate_t{}} {};
  jthread(jthread&& other) noexcept
      : m_thread{std::move(other.m_thread)}, m_stop{std::move(other.m_stop)} {};

  /// Runs f(args...) in a new thread, if f can also take a stop_token as
  /// first argument is called with the token on this jthread
  template <typename Function, typename... Args>
  explicit jthread(Function&& f, Args&&... args) {
    if constexpr (std::is_invocable_v<std::decay_t<Function>, stop_token,
                                      std::decay_t<Args>...>) {
      /// std::thread decay copies everything including the token
      m_thread = std::thread{[](auto fn, stop_token token, auto... a) {
                               std::invoke(std::move(fn), std::move(token),
                                           std::move(a)...);
                             },
                             std::forward<Function>(f), m_stop.get_token(),
                             std::forward<Args>(args)...};
    } else {
      m_thread =
          std::thread{std::forward<Function>(f), std::forward<Args>(args)...};
    }
  };

  jthread(const jthread&) = delete;

  /// A joinable thread asks the thread to stop and it
  /// waits for it
  ~jthread() {
    if (joinable()) {
      request_stop();
      join();
    }
  };

  jthread& operator=(jthread&& other) noexcept {
    if (this != &other) {
      if (joinable()) {
        request_stop();
        join();
      }
      m_thread = std::move(other.m_thread);
      m_stop = std::move(other.m_stop);
    }
    return *this;
  };

  [[nodiscard]] bool joinable() const noexcept;
  [[nodiscard]] dss::jthread::id get_id() const noexcept;
  [[nodiscard]] static unsigned int harware_concurrency() noexcept;

  void join();
  void detatch();

  void swap(dss::jthread& other) noexcept;
  bool request_stop() noexcept { return m_stop.request_stop(); };

 private:
  std::thread m_thread;
  stop_source m_stop;
};

} // namespace dss

#endif // JTHREAD_H
