#pragma once

#include <atomic>
#include <concepts>
#include <memory>
#include <mutex>
#include <stdexcept>
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

// Immortal singleton
template <typename T> class ImmortalSingleton {
protected:
  // clang-format off
  class Key { explicit Key() = default; friend class ImmortalSingleton; };
  // clang-format on

public:
  ImmortalSingleton(const ImmortalSingleton &) = delete;
  ImmortalSingleton &operator=(const ImmortalSingleton &) = delete;
  static T &instance() {
    alignas(T) static unsigned char storage[sizeof(T)];
    static T *p = ::new (static_cast<void *>(storage)) T{Key{}};
    return p;
  }
};

// Singleton with arguments on startup
class Settings {
public:
  // Must be called once and before instance(), it can be called only once
  static void init(std::string path) {}
  static Settings &instance() {
    // clang-format off
    if (!inst_) throw std::logic_error("Settings::init() not called");
    return *inst_;
    //clang-format on
  }

  const std::string& path() const { return path_; }
  Settings(const Settings&) = delete;
  Settings& operator=(const Settings&) = delete;

private:
  explicit Settings(std::string p) : path_{std::move(p)} {}
  std::string path_{};
  static inline std::once_flag flag_;
  static std::unique_ptr<Settings> inst_;
};


