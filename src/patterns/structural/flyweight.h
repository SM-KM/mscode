#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

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

  Handle intern(const T &value) {
    std::lock_guard lk{mu_};
    return Handle(&*set_.insert(value).first);
  };
  // clang-format off
  std::size_t size() const { std::lock_guard lk{mu_}; return set_.size(); };
  // clang-format on
};

struct TreeModel {
  std::string species;
  int leaf_color;
  std::vector<std::byte> mesh;
  friend bool operator==(const TreeModel &, const TreeModel &) = default;
};

template <> struct std::hash<TreeModel> {
  // clang-format off
  std::size_t operator()(const TreeModel &m) const noexcept {
    return std::hash<std::string>{}(m.species) * 31 + static_cast<std::size_t>(m.leaf_color);
  }
};
static_assert(Internable<TreeModel> && Internable<std::string>);

// clang-format off
struct Tree { Interner<TreeModel>::Handle model; float x, y, scale; };
struct FatTree { TreeModel model; float x, y, scale; };
// clang-format on

template <std::equality_comparable T, std::unsigned_integral Id = std::uint8_t>
class FlyweightTable {
  std::vector<T> items_;

public:
};
