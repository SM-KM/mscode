#pragma once

#include <concepts>
#include <memory>
#include <ostream>
#include <type_traits>
#include <utility>

/// Type erasure gives you runtime polymorphism without requiring types to
/// inherit from a base class: anything that supports that needed operations can
/// be stored behind one concrete wrapper type

inline void draw(int v, std::ostream &os) { os << ""; }

template <typename T>
concept DrawableLike = requires(const T &t, std::ostream &os) { draw(t, os); };

class AnyDrawable {
  struct Concept {
    virtual ~Concept() = default;
    virtual std::unique_ptr<Concept> clone() const = 0;
    virtual void do_draw(std::ostream &) const = 0;
  };

  template <typename T> struct Model final : Concept {
    T value;
    explicit Model(T v) : value{std::move(v)} {}
    // clang-format off
    std::unique_ptr<Concept> clone() const override { return std::make_unique<Model>(*this); }
    void do_draw(std::ostream& os) const override { draw(value, os); }
  };
  std::unique_ptr<Concept> p_;

public:
  template<typename T>
      requires (!std::same_as<std::remove_cvref<T>, AnyDrawable>) && DrawableLike<std::decay_t<T>> &&
      std::copy_constructible<std::decay_t<T>>
      AnyDrawable(T&& t) : p_{std::make_unique<Model<std::decay_t<T>>>(std::forward<T>(t))} {}
};
