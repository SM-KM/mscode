#pragma once

#include <concepts>
#include <string>
#include <utility>
#include <vector>
#include <version>
#if __cplusplus > 202002L && __has_include(<expected>)
#include <expected>
#endif

/// Fluent Builder and Builder patterns
/// @brief a way of build/construct complex objects in a linear,
/// simpler and scalable way
/// Classic builder + Director pattern
struct Computer {
  std::string cpu, gpu;
  int ram_gb{0};
  std::vector<std::string> disks;
};

/// @brief It is a product builder for any product if it can be reseted and
/// configured in steps and it yields a product
template <typename B, typename Product>
concept ProductBuilder = requires(B b) {
  b.reset();
  { b.build() } -> std::same_as<Product>;
};

template <typename B>
concept ComputerBuilder =
    ProductBuilder<B, Computer> && requires(B b, std::string s, int n) {
      b.cpu(s);
      b.gpu(s);
      b.ram(n);
      b.add_rank(s);
    };

class GamingBuilder {
  Computer c_;

 public:
  void reset() {};
  Computer build() { return std::exchange(c_, {}); };
};

class OfficeBuilder {};

/// @brief Any builder that satisfies the concept works
template <ComputerBuilder B>
class Director {
  B& b_;

 public:
  explicit Director(B& b) : b_{b} {}
  Computer high_end() {};
  Computer budget() {};
};
