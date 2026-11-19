#pragma once

#include <atomic>
#include <cstddef>
#include <string>
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
};
