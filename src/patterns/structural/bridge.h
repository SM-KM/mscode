#pragma once

#include <concepts>
#include <memory>
#include <string_view>
#include <type_traits>

// Bridge structural pattern

template <typename B>
concept GfxBackend = requires(B &b, std::string_view s, float f) {
  b.draw_quad(f, f, f, f);
  b.draw_text(s, f, f);
  { b.name() } -> std::convertible_to<std::string_view>;
};

struct IGfxBackend {
  virtual ~IGfxBackend() = default;
  virtual void draw_quad(float x, float y, float w, float h) = 0;
  virtual void draw_text(std::string_view text, float x, float y) = 0;
  virtual std::string_view name() const = 0;
};

struct GlBackend final : IGfxBackend {
public:
  void draw_quad(float x, float y, float w, float h) override {}
  void draw_text(std::string_view text, float x, float y) override {}
  std::string_view name() const override { return std::string_view{}; }
};

struct VkBackend final : IGfxBackend {
public:
  void draw_quad(float x, float y, float w, float h) override {}
  void draw_text(std::string_view text, float x, float y) override {}
  std::string_view name() const override { return std::string_view{}; }
};

struct ThirdPartyBackend {
  int calls{0};
  void draw_quad(float, float, float, float) { ++calls; }
  void draw_text(std::string_view, float, float) { ++calls; }
  std::string_view name() const { return "somthing lol"; }
};

/// Classic runtime bridge, virtual implementor interface, backend swappable at
/// runtime
class Drawable {
protected:
  IGfxBackend *gfx_;

public:
  explicit Drawable(IGfxBackend &g) : gfx_{&g} {}
  virtual ~Drawable() = default;
  virtual void draw() const = 0;
  void set_backend(IGfxBackend &g) {}
};

class Sprite : public Drawable {
  float x_, y_, w_, h_;

public:
  /// the backend impl that it wants
  Sprite(IGfxBackend &g, float x, float y, float w, float h)
      : Drawable{g}, x_{x}, y_{y}, w_{w}, h_{h} {}
  void draw() const override { gfx_->draw_quad(x_, y_, w_, h_); };
};

/// Static bridge, abstraction templated on a concept-checked backend
template <GfxBackend B> class StaticSprite {
  B &gfx_;
  float x_, y_, w_, h_;

public:
  StaticSprite(B &g, float x, float y, float w, float h)
      : gfx_{g}, x_{x}, y_{y}, w_{w}, h_{h} {}
  void draw() const { gfx_.draw_quad(x_, y_, w_, h_); }
};

/// Non-intrusive bridge, BackendRef - type erased reference to ANY type that
/// fits the concept, no common base class needed
class BackendRef {
  void *obj_;
  void (*quad_)(void *, float, float, float, float);
  void (*text_)(void *, std::string_view, float, float);
  std::string_view (*name_)(void *);

  // clang-format off
public:
  template <GfxBackend B> requires(!std::same_as<std::remove_cvref_t<B>, BackendRef>)
  BackendRef(B &b)
      : obj_{std::addressof(b)},
        quad_([](void *o, float x, float y, float w, float h) { static_cast<B *>(o)->draw_quad(x, y, w, h); }),
        text_([](void* o, std::string_view t, float x, float y) {static_cast<B*>(o)->draw_text(t, x, y);} ),
	name_([](void* o) -> std::string_view { return static_cast<B*>(o)->name(); }) {}

    void draw_quad(float x, float y, float w, float h) { quad_(obj_, x, y, w, h); }
    void draw_text(std::string_view t, float x, float y) { text_(obj_, t, x, y); }
    std::string_view name() const { return name_(obj_); }
  // clang-format on
};
