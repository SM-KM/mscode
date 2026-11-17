#pragma once

#include <algorithm>
#include <concepts>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

/// Decorator pattern
/// A decorator wraps in object that has the same interface and adds Behavior
/// before and after delegating, so you can stack features at runtime instead of
/// subclassing every combination.

// clang-format off
/// Classic decorator
struct IWeapon {
  virtual ~IWeapon() = default; virtual int damage() const = 0;
  virtual std::string describe() const = 0;
};

// clang-format on
struct Sword final : IWeapon {
public:
  int damage() const override { return 10; }
  std::string describe() const override { return "Sword"; }
};

// Base decorator
class WeaponDecorator : public IWeapon {
protected:
  std::unique_ptr<IWeapon> inner_;

public:
  explicit WeaponDecorator(std::unique_ptr<IWeapon> w) : inner_{std::move(w)} {}
  int damage() const override { return inner_->damage(); }
  std::string describe() const override { return inner_->describe(); }
};

// clang-format off
/// Includes a flat bonus on the damage, due to the "fire" aspect
class Fire final : public WeaponDecorator {
public:
  using WeaponDecorator::WeaponDecorator;
  int damage() const override { return inner_->damage() + 5; }
  std::string describe() const override { return inner_->describe() + " +Fire"; }
};

class Poison final : public WeaponDecorator {               
    int per_tick_;
public:
    Poison(std::unique_ptr<IWeapon> w, int per_tick) : WeaponDecorator(std::move(w)), per_tick_(per_tick) {}
    int damage() const override { return inner_->damage() + per_tick_ * 3; }
    std::string describe() const override { return inner_->describe() + " +Poison(" + std::to_string(per_tick_) + ")"; }
};

// Pipe sintax -> w | with<Fire>() | with<Poison>(10)
template <typename D, typename... Args> struct Adder { std::tuple<Args...> args; };
template <typename D, typename... Args>
requires std::derived_from<D, WeaponDecorator> && std::constructible_from<D, std::unique_ptr<IWeapon>, Args...>
Adder<D, std::decay_t<Args>...> with(Args&&... a) {
    return std::tuple<std::decay_t<Args>...>(std::forward<Args>(a)...);
}

template<typename D, typename... Args>
std::unique_ptr<IWeapon> operator|(std::unique_ptr<IWeapon> w, Adder<D, Args...> add) {
    return std::apply([&](auto&&... a){ return std::make_unique<D>(std::move(w), a...); },
	   std::move(add.args));
}

template<std::derived_from<IWeapon> w, typename... Args>
std::unique_ptr<IWeapon> make(Args&&... a) { return std::make_unique<w>(std::forward<Args>(a)...); }
// clang-format on

/// Static decorator
template <typename W>
concept Weapon = requires(const W &w) {
  { w.damage() } -> std::convertible_to<int>;
  { w.describe() } -> std::convertible_to<std::string>;
};

struct PlainSword {
  int damage() const { return 10; }
  std::string describe() const { return "Sword"; }
};

template <Weapon W> class SFire {
  W inner_;

public:
  explicit SFire(W w) : inner_{std::move(w)} {}
  int damage() const { return inner_.damage() + 5; }
  std::string describe() const { return inner_.describe() + " +Fire"; }
};

static_assert(Weapon<PlainSword> && Weapon<SFire<PlainSword>>);
// Decorator costs no memory
static_assert(sizeof(SFire<PlainSword>) == sizeof(PlainSword));
