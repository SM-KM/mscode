#ifndef JTHREAD_H
#define JTHREAD_H

#include <thread>
namespace dss {

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
