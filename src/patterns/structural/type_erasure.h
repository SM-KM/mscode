#pragma once

#include <concepts>
#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
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

      AnyDrawable(const AnyDrawable& o) : p_{o.p_->clone()} {}
      AnyDrawable(AnyDrawable&&) noexcept = default;
      AnyDrawable& operator=(AnyDrawable o) noexcept { p_ = std::move(o.p_); return *this; }

      friend void draw(const AnyDrawable& d, std::ostream& os) { d.p_->do_draw(os); }
};

struct Circle { double r; };
struct Square { double side; };
void draw(const Circle& c, std::ostream& os);
void draw(const Square& s, std::ostream& os);

namespace thirdparty {
struct Sprite { std::string file; };	
void draw(const Sprite& s, std::ostream& os);
}
// clang-format on

static_assert(DrawableLike<Circle> && DrawableLike<int> &&
              DrawableLike<thirdparty::Sprite> && DrawableLike<AnyDrawable>);
static_assert(!DrawableLike<std::string>);

/// Manual vtable + small-buffer optimization
template <typename T>
concept ShapeLike = std::copy_constructible<T> && requires(const T &t) {
  { t.area() } -> std::convertible_to<double>;
  { t.name() } -> std::convertible_to<std::string>;
};

class AnyShape {
  static constexpr std::size_t kBuf{32};
  struct Storage {
    alignas(std::max_align_t) std::byte buf{kBuf};
    void *heap{nullptr};
  };

  struct VTable {
    double (*area)(const Storage &);
    std::string (*name)(const Storage &);
    void (*copy)(const Storage &from, Storage &to);
    void (*move)(Storage &from, Storage &to) noexcept;
    void (*destroy)(Storage &) noexcept;
    bool inline_storage;
  };

  template <typename T> struct Ops {
    static constexpr bool KInline = sizeof(T) <= kBuf &&
                                    alignof(T) <= alignof(std::max_align_t) &&
                                    std::is_nothrow_move_constructible_v<T>;
  };
};
