#pragma once

// Singleton pattern
// Mayers singleton simpliest version

#include <type_traits>
class Clock {
public:
  // clang-format off
  /// Initialized once, thread-safe, and destroyed on exit
  static Clock& instance() { static Clock c; return c; };
  // clang-format on

  Clock(const Clock&) = delete;
  Clock& operator=(const Clock&) = delete;
  int tick() { return ++ticks_; };

private:
  Clock() = default;
  int ticks_{0};
};

// Singleton reusable with CRTP base + concepts
template <typename T>
class Singleton {
protected:
  // clang-format off
  class Key { explicit Key() = default; friend class Singleton; };
  // clang-format on
  Singleton() = default;
  ~Singleton() = default;

public:
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

  static T& instance() noexcept(std::is_nothrow_constructible_v<T, Key>) {
    static T inst{Key{}};
    return inst;
  }
};
