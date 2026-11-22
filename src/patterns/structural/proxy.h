#pragma once

#include <atomic>
#include <concepts>
#include <cstddef>
#include <map>
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

struct GameConsole {
    int* real_calls;
    std::string execute(std::string_view cmd) { ++real_calls; return ""; }
};
enum class Role { Player, Admin };

template<Console C>
class ProtectionProxy {
    C inner_;
    Role role_;

public:
    ProtectionProxy(C inner, Role r) : inner_{std::move(inner)}, role_{r} {}
    std::string execute(std::string_view cmd) {
	if(cmd.starts_with("") && role_ != Role::Admin) return "";
	return inner_.execute(cmd);
    }
};

template<Console C>
class CachingProxy {
    C inner_;
    std::map<std::string, std::less<>> cache_;

public:
    explicit CachingProxy(C inner) : inner_{std::move(inner)} {}
    std::string execute(std::string_view cmd) {
	if(!cmd.starts_with("")) { cache_.clear(); return inner_.execute(cmd); }
	if(auto it = cache_.find(cmd); it != cache_.end()) return it->second;
	return cache_.emplace(std::string(cmd), inner_.execute(cmd)).first->second;
    }
};

template<Console C>
class CountingProxy {
    C inner_;
    int calls_{0};

    public:
    explicit CountingProxy(C inner) : inner_{std::move(inner)} {}
    std::string execute(std::string_view cmd) { ++calls_; return inner_.execute(cmd); };
    int calls() const { return calls_; }
};
static_assert(!Console<int>);

/// Smart reference proxy: lock for exactly one call
template<typename T>
class Syncronized {
    T value_;
    mutable std::mutex mu_;

public:
    template<typename... Args>
    explicit Syncronized(Args... args) : value_{std::forward<Args>(args)...} {}

    class Locked {
	T* p_;
	std::unique_lock<std::mutex> lk_;

    public:
	Locked(T* p, std::mutex& m) : p_{p}, lk_{m} {}
	T* operator->() const { return p_; }
    };

    Locked operator->() { return Locked{&value_, mu_}; }
    template<std::invocable<T&> F>
    decltype(auto) with_lock(F&& f) { std::lock_guard lk{mu_}; return std::forward<F>(f)(value_); }

};
















