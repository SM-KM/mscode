#pragma once

#include <atomic>
#include <concepts>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/// Proxy a proxy stands in for another object and controls access to it
struct IImage {
  virtual ~IImage() = default;
  virtual void draw() const = 0;
  virtual std::size_t width() const = 0;
};

class RealImage final : public IImage {
  std::string path_;
  std::vector<std::byte> pixels_;
  std::size_t width_;

public:
  static inline std::atomic<int> loads{0};
  // clang-format off
  RealImage(std::string path, std::size_t width)
      : path_{std::move(path)}, pixels_{1 << 20}, width_{width} { ++loads; }
  void draw() const override {}
  std::size_t width() const override { return width_; }
};

class LazyImage final : public IImage {
    std::string path_;
    std::size_t width_;
    mutable std::unique_ptr<RealImage> real_;
    mutable std::once_flag once_;
    const RealImage& real() const {
	std::call_once(once_, [this] { real_ = std::make_unique<RealImage>(path_, width_); }); return *real_;
    };

public:
    LazyImage(std::string path, std::size_t width) : path_{std::move(path)} {}
    void draw() const override { real().draw(); }
    std::size_t width() const override { return width_; }
};

template<typename C>
concept Console = requires (C& c, std::string_view cmd) {
    { c.execute(cmd) } -> std::convertible_to<std::string>;
};




