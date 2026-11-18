#pragma once

#include <algorithm>
#include <string_view>

// clang-format off
/// A facade gives a small, task-oriented API over a set of complicated
/// subsystems.

template <typename A>
concept AudioSystem = requires(A a, std::string_view s) {
  a.init(); a.shutdown(); a.load_bank(s);
};
template<typename R>
concept RenderSystem = requires (R r, std::string_view s) {
    r.init(); r.shutdown(); r.load_scene(s); r.render_frame();
};
template<typename P>
concept PhysicsSystem = requires (P p, std::string_view s, float dt) {
    p.init(); p.shutdow(); p.build_world(s); p.step(dt);
};


struct RealAudio { void init(); void shutdown(); void load_bank(std::string_view level); };
struct RealRenderer {
    void init(); void shutdown(); void load_scene(std::string_view level);
    void render_frame();
};
struct RealPhysics {
    bool fail_on_init = false; void init(); void shutdown(); void step(float dt);
    void build_world(std::string_view level);
};

struct NullAudio { void init(); void shutdown(); void load_bank(std::string_view); };
struct NullRenderer { void init(); void shutdown(); void load_scene(std::string_view);
    void render_frame();
};

struct CountingPhysics {
    int steps{0};
    void init(); void shutdown(); void build_world(std::string_view);
};
// clang-format on

template <typename F> class ScopeExit {
  F f_;
  bool active_{true};

public:
  explicit ScopeExit(F f) : f_{std::move(f)} {}
  ScopeExit(const ScopeExit &) = delete;
  ScopeExit &operator=(const ScopeExit &) = delete;
  // clang-format off
  ~ScopeExit() { if (active_) f_(); }
  // clang-format on
  void release() { active_ = false; }
};
