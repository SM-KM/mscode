// clang-format off
#pragma once

#include <cmath>
#include <string_view>

/// Strategy makes an algorithm interchangeable, the context holds the whwat to
/// do, the strategy decide the how
struct Vec2 { float x{0}, y{0}; };
inline float dist(Vec2 a, Vec2 b) { return std::hypot(a.x - b.x, a.y - b.y); } 
inline Vec2 step_toward(Vec2 a, Vec2 b, float speed) {
    float d{dist(a, b)};
    if (d <= speed) return b;
    return {a.x + (b.x - a.x) / d * speed, a.y + (b.y - a.y) / d * speed};
}

struct MovementStrategy {
    virtual ~MovementStrategy() = default;
    virtual Vec2 next(Vec2 self, Vec2 player) = 0;
    virtual std::string_view name() const = 0;
};
    
