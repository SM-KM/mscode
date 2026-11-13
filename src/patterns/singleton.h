#pragma once

#include <atomic>
#include <concepts>
#include <mutex>
#include <string>
#include <type_traits>

// Singleton pattern
// Mayers singleton simpliest version
class Clock {
public:
  // clang-format off
  /// Initialized once, thread-safe, and destroyed on exit
  static Clock& instance() { static Clock c; return c; };
  // clang-format on

  Clock(const Clock &) = delete;
  Clock &operator=(const Clock &) = delete;
  int tick() { return ++ticks_; };

private:
  Clock() = default;
  int ticks_{0};
};

// Singleton reusable with CRTP base + concepts
template <typename T> class Singleton {
protected:
  // clang-format off
  class Key { explicit Key() = default; friend class Singleton; };
  // clang-format on
  Singleton() = default;
  ~Singleton() = default;

public:
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  static T &instance() noexcept(std::is_nothrow_constructible_v<T, Key>) {
    static T inst{Key{}};
    return inst;
  }
};

// clang-format off
template<typename T>
concept SingletionType = std::derived_from<T, Singleton<T>> && !std::copy_constructible<T> && !std::move_constructible<T> &&
!std::default_initializable<T> && requires { { T::instance() } -> std::same_as<T&>; };
// clang-format on

template <SingletionType T> T &get() { return T::instance(); }

class Logger final : public Singleton<Logger> {
public:
  explicit Logger(Key) {}
  void log(const std::string &m) {};
  int count() const { return count_.load(); };
  std::string last() const {
    std::lock_guard lk{m_};
    return last_;
  }

private:
  mutable std::mutex m_;
  std::atomic<int> count_{0};
  std::string last_;
};
