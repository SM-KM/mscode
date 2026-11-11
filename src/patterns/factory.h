#pragma once

#include <concepts>
#include <string>
#include <type_traits>

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
// clang-format on

/// Simple factory

enum class Kind { Goblin, Orc, Dragon };

/// Concept: Derived is concrete Base that it can be build from Args
template <typename T, typename Base, typename... Args>
concept Creatable = std::derived_from<T, Base> && std::constructible_from<T, Args...> && !std::is_abstract_v<T>;
