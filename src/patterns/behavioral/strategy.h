// clang-format off
#pragma once

#include <cmath>
#include <memory>
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

struct MovementStrategy { virtual ~MovementStrategy() = default;
    virtual Vec2 next(Vec2 self, Vec2 player) = 0;
    virtual std::string_view name() const = 0;
};
    
class Chase final : public MovementStrategy {
public:
    Vec2 next(Vec2 self, Vec2 player) override { return step_toward(self, player, 1.5f); }
    std::string_view name() const override { return ""; }
};

class Flee final : public MovementStrategy {
public:
    Vec2 next(Vec2 self, Vec2 player) override {
	Vec2 away{self.x + (self.x - player.x), self.y + (self.y - player.y)};
	return step_toward(self, away, 2.0f);
    }
    std::string_view name() const override { return ""; }
};

enum class Mode { Patrol, Chase, Flee };
inline std::unique_ptr<MovementStrategy> make_strategy(Mode m) {
    switch (m) {
        case Mode::Chase: return std::make_unique<Chase>();
        case Mode::Flee:  return std::make_unique<Flee>();
        default:          return std::make_unique<Chase>();
    }
}

class Enemy {                                                   // the context
    Vec2 pos_{};
    int hp_ = 100;
    Mode mode_ = Mode::Patrol;
    std::unique_ptr<MovementStrategy> mv_ = make_strategy(mode_);
public:
    void set_hp(int hp) { hp_ = hp; }
    Vec2 pos() const { return pos_; }
    std::string_view strategy() const { return mv_->name(); }
    void update(Vec2 player) {
        Mode want = hp_ < 30 ? Mode::Flee : dist(pos_, player) < 6 ? Mode::Chase : Mode::Patrol;
        if (want != mode_) { mode_ = want; mv_ = make_strategy(want); }   // swap strategy at runtime
        pos_ = mv_->next(pos_, player);
    }
};
