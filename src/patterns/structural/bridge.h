#pragma once

#include <concepts>
#include <string_view>

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
  Sprite(IGfxBackend &g, float x, float y, float w, float h)
      : Drawable{g}, x_{x}, y_{y}, w_{w}, h_{h} {}
  void draw() const override { gfx_->draw_quad(x_, y_, w_, h_); };
};

/// Static bridge, abstraction templated on a concept-checked backend

/// Non-intrusive bridge, BackendRef - type erased reference to ANY type that
/// fits the concept, no common base class needed
