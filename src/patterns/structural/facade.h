#pragma once

#include <algorithm>
#include <stdexcept>
#include <string_view>
#include <sys/socket.h>

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

template <AudioSystem A, RenderSystem R, PhysicsSystem P> class Engine {
  A audio_;
  R renderer_;
  P physics_;
  bool started_{false};

public:
  // clang-format off
  Engine(A a, R r, P p)
      : audio_{std::move(a)}, renderer_{std::move(r)}, physics_{std::move(p)} {}
  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;
  ~Engine() { shutdown(); }

  void start() {
      if(started_) return;
      audio_.init(); ScopeExit undo_audio{[&] { audio_.shutdown(); }};
      renderer_.init(); ScopeExit undo_render{[&] { renderer_.shutdown(); }};
      physics_.init(); undo_render.release(); undo_audio.release(); started_ = true;
  }

  void run(int frames, float dt = 1.0f / 60) {
      require_started();
      for (int i = 0; i < frames; ++i) { physics_.step(dt); renderer_.render_frame(); }
  }

  void load_level(std::string_view name) {
      require_started(); audio_.load_bank(name); renderer_.load_scene(name);
      physics_.build_world(name);
  }

  void shutdown() {
    if (!started_) return; physics_.shutdown(); renderer_.shutdown(); audio_.shutdown();
    started_ = false;
  }

private:
  void require_started() const { if(!started_) throw std::logic_error("engine not started"); };
};
