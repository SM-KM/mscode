#pragma once

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

// clang-format on
