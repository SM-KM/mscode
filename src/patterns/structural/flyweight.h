#pragma once

#include <concepts>
#include <functional>
#include <mutex>
#include <unordered_set>

// clang-format off
/// Flyweight 

template<typename T>
concept Internable  = std::constructible_from<T> && std::equality_comparable<T> &&
    requires(const T& t) { { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>; };

// clang-format on
template <Internable T> class Interner {
  std::unordered_set<T> set_;
  mutable std::mutex mu_;

public:
  class Handle {
    const T *p_ = nullptr;
    explicit Handle(const T *p) : p_{p} {}
    friend class Interner;

  public:
    Handle() = default;
    const T &operator*() const { return *p_; }
    const T *operator->() const { return p_; }
    friend bool operator==(Handle a, Handle b) { return a.p_ == b.p_; }
  };
};
