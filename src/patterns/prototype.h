#pragma once

#include <concepts>
#include <memory>
#include <string>

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
  Monster(std::string name, int hp, polymorphic_ptr<Weapon> w) {}

protected:
  Monster(const Monster&) = default;
  Monster& operator=(const Monster&) = delete;

private:
  std::string name_;
  int hp_;
  polymorphic_ptr<Weapon> weapon_;
};
