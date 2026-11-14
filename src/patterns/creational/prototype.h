#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

/// Prototype pattern
/// a type qualifies if clone() returns something convertible to
/// std::unique_ptr<T>

// T is a Prototype if it can produce an owning copy of itself as a T

template <typename T>
concept Prototype = requires(const T& t) {
  { t.clone() } -> std::convertible_to<std::unique_ptr<T>>;
};

template <Prototype Base, typename Derived>
class Cloneable : public Base {
  using Base::Base;
  std::unique_ptr<Base> clone() const override {
    static_assert(std::copy_constructible<Derived>, "Derived must be copy_constructible");
    return std::make_unique<Derived>(static_cast<const Derived&>(*this));
  }
};

template <Prototype T>
class polymorphic_ptr {
  std::unique_ptr<T> p_;

public:
  polymorphic_ptr() = default;
  template <std::derived_from<T> D>
  polymorphic_ptr(std::unique_ptr<D> p) : p_{std::move(p)} {}

  polymorphic_ptr(const polymorphic_ptr& o) : p_{o.p_ ? o.p_.clone() : nullptr} {}
  polymorphic_ptr(polymorphic_ptr&&) noexcept = default;
  // clang-format off
  polymorphic_ptr& operator=(polymorphic_ptr o) noexcept { p_.swap(o.p_); return *this; }
  // clang-format on

  T* operator->() const { return p_.get(); }
  T& operator*() const { return *p_; }
  explicit operator bool() const { return static_cast<bool>(p_); }
};

class Weapon {
public:
  Weapon(std::string name, int damage) : name_{std::move(name)}, damage_{damage} {}
  virtual ~Weapon() = default;
  virtual std::unique_ptr<Weapon> clone() const = 0;
  const std::string& name() const { return name_; };
  int damage() const { return damage_; };
  void set_damage(int d) { damage_ = d; }

protected:
  Weapon(const Weapon&) = default;
  Weapon& operator=(const Weapon&) = delete;

private:
  std::string name_;
  int damage_;
};

// clang-format off
struct Sword : Cloneable<Weapon, Sword> { using Cloneable::Cloneable; };
struct Bow : Cloneable<Weapon, Bow> { using Cloneable::Cloneable; };
// clang-format on

class Monster {
public:
  Monster(std::string name, int hp, polymorphic_ptr<Weapon> w)
      : name_{std::move(name)}, hp_{hp}, weapon_{std::move(w)} {}
  virtual ~Monster() = default;
  virtual std::unique_ptr<Monster> clone() const = 0;

  void rename(std::string n) { name_ = std::move(n); }
  void set_hp(int h) { hp_ = h; }
  Weapon& weapon() { return *weapon_; }
  std::string describe();

protected:
  Monster(const Monster&) = default;
  Monster& operator=(const Monster&) = delete;

private:
  std::string name_;
  int hp_;
  polymorphic_ptr<Weapon> weapon_;
};

// clang-format off
struct Goblin : Cloneable<Monster, Goblin> { using Cloneable::Cloneable; };
struct Archer : Cloneable<Monster, Archer> { using Cloneable::Cloneable; };
// clang-format on
static_assert(Prototype<Monster> && Prototype<Weapon>);
static_assert(!Prototype<int>);

template <Prototype Base>
class PrototypeRegistry {
  std::unordered_map<std::string, std::unique_ptr<Base>> protos_;

public:
  template <std::derived_from<Base> D>
  void add(std::string key, std::unique_ptr<D> proto) {
    protos_[std::move(key)] = std::move(proto);
  }

  std::unique_ptr<Base> spawn(const std::string& key) const {
    auto it = protos_.find(key);
    return it == protos_.end() ? nullptr : it->second->clone();
  }
};

// clang-format off
template<std::copy_constructible T>
std::unique_ptr<T> clone_value(const T& v) { return std::make_unique<T>(v); }
