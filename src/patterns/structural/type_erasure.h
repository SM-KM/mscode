#pragma once

#include <memory>
#include <ostream>

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
  };
};
