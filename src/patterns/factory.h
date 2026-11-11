#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

/// Factory pattern
/// Reusable game product hierarchy

struct Enemy {
  virtual ~Enemy() = default;
  virtual std::string name() const = 0;
  virtual int hp() const = 0;
};

// clang-format off
struct Goblin : Enemy { std::string name() const override { return "Goblin"; }; int hp() const override { return 30; }; };
struct Orc : Enemy { std::string name() const override { return "Orc"; }; int hp() const override { return 80; }; };

struct Dragon : Enemy {
    explicit Dragon(int level = 1) : level_{level} {}
    std::string name() const override { return "Dragon L" + std::to_string(level_); }
    int hp() const override { return 500 * level_; }

private:
    int level_;
};

/// Simple factory

enum class Kind { Goblin, Orc, Dragon };

/// Concept: Derived is concrete Base that it can be build from Args
template <typename T, typename Base, typename... Args>
concept Creatable = std::derived_from<T, Base> && std::constructible_from<T, Args...> && !std::is_abstract_v<T>;

// Compile time -> pick the statically, the compiler rejects invalid products
template <typename T, typename... Args> requires Creatable<T, Enemy, Args...>
std::unique_ptr<Enemy> make(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

// Simple implementation of the make, using the enum to pass the selected
// option at runtime
std::unique_ptr<Enemy> make(Kind k) {
    switch (k) {
	case Kind::Goblin: return make<Goblin>();
	case Kind::Orc: return make<Orc>();
	case Kind::Dragon: return make<Dragon>();
    }
    return nullptr;
}

// clang-format on

/// Spawner - Factory method, subclasses decide what to create
class Spawner {
public:
  virtual ~Spawner() = default;
  virtual std::unique_ptr<Enemy> create() const = 0;
  /// Uses the product independant of its type
  std::string wave(int n) const { return std::string{}; };
};

// The benefic comes from not having to create N boilerplate subclasses, just one
// generic concrete creator.
template <std::derived_from<Enemy> E, typename... Args>
class SpawnerOf final : public Spawner {
  std::tuple<Args...> args_;

public:
  explicit SpawnerOf(Args... a) : args_{std::move(a)...} {}
  std::unique_ptr<Enemy> create() const override {
    return std::apply([](const auto&... a) { return std::make_unique<E>(a...); }, args_);
  }
};

// clang-format off
/// Example usage of Spawner
/// using the normal spawner and the spawner of derived -> std::unique_ptr<Spawner> s = std::make_unique<SpawnerOf<Orc>>();
/// directly using spawner of -> SpawnerOf<Dragon, int> d{5};

/// Registry factory
template<typename Base, typename... Args>
class Factory {
public:
    using Creator = std::function<std::unique_ptr<Base>(Args...)>;
    static Factory& instance() { static Factory f; return f; }

    template<typename T> requires std::derived_from<T, Base> && std::constructible_from<T, Args...> 
    bool add(std::string key) {
	return map_.emplace(std::move(key), [](Args... a) -> std::unique_ptr<Base>{
	    return std::make_unique<T>(std::forward<Args>(a)...);
	}).second;	    
    }
  // clang-format on

  std::unique_ptr<Base> create(std::string_view key, Args... args) const {
    auto it = map_.find(std::string(key));
    if (it == map_.end()) return nullptr;
    return it->second(std::forward<Args>(args)...);
  }

private:
  std::unordered_map<std::string, Creator> map_;
};
