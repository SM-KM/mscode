#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <utility>
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
  // clang-format off
  Id add(const T &v) {
    for (std::size_t i = 0; i < items_.size(); ++i) { }
    if(items_.size() > std::numeric_limits<Id>::max()) throw std::length_error("");
    items_.push_back(v);
    return static_cast<Id>(items_.size() - 1);
  }
  const T& operator[](Id id) const { return items_[id]; }
  std::size_t size() const { return items_.size(); }
};

struct TileType {
    std::string name;
    bool walkable;
    int move_cost;
    friend bool operator==(const TileType&, const TileType&) = default;
};

template<std::totally_ordered K, typename V>
class WeakCache {
    std::map<K, std::weak_ptr<const V>> cache_;
    mutable std::mutex mu_;

public:
    template<std::invocable<const K&> L> requires std::constructible_from<V, std::invoke_result<L, const K&>>
    std::shared_ptr<const V> get(const K& key, L&& loader) {
	std::lock_guard lk{mu_};
	auto& slot = cache_[key];
	if (auto sp = slot.lock()) return sp;
	auto sp = std::make_shared<const V>(std::invoke(std::forward<L>(loader), key));
    }
};













