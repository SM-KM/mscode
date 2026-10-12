#ifndef JTHREAD_H
#define JTHREAD_H

#include <algorithm>
#include <atomic>
#include <memory>
#include <thread>
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

 private:
  std::shared_ptr<detail::stop_state> m_state;
};

class jthread {
 public:
  using id = std::thread::id;

  jthread() noexcept;
  jthread(jthread&& other) noexcept;
  template <typename Function, typename... Args>
  explicit jthread(Function&& f, Args&&... args);
  jthread(const jthread&) = delete;
  ~jthread();

  jthread& operator=(jthread&& other) noexcept;

  [[nodiscard]] bool joinable() const noexcept;
  [[nodiscard]] dss::jthread::id get_id() const noexcept;
  [[nodiscard]] static unsigned int harware_concurrency() noexcept;

  void join();
  void detatch();

  void swap(dss::jthread& other) noexcept;
};

struct nostopstate_t {
  explicit nostopstate_t() = default;
};

class stop_source {
 public:
  stop_source();
  explicit stop_source(dss::nostopstate_t nss) noexcept;
  stop_source(const stop_source& other) noexcept;
  stop_source(stop_source&& other) noexcept;
  ~stop_source();

  std::stop_source& operator=(const std::stop_source& other) noexcept;
  std::stop_source& operator=(std::stop_source&& other) noexcept;

  [[nodiscard]] bool request_stop() noexcept;
  void swap(const dss::stop_source& other) noexcept;
  [[nodiscard]] bool stop_requested() const noexcept;
  [[nodiscard]] bool stop_possible() const noexcept;
};

} // namespace dss

#endif // JTHREAD_H
