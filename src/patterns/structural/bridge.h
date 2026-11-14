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

/// Classic runtime bridge, virtual implementor interface, backend swappable at
/// runtime

/// Static bridge, abstraction templated on a concept-checked backend

/// Non-intrusive bridge, BackendRef - type erased reference to ANY type that
/// fits the concept, no common base class needed
