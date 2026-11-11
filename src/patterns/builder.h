#pragma once
// INFO: change the dss from the standard with the ones
// implemented in dss

#include <chrono>
#include <concepts>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <version>

#if __cplusplus > 202002L && __has_include(<expected>)
#include <expected>
#endif

/// Builder patterns

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
concept ComputerBuilder = ProductBuilder<B, Computer> && requires(B b, std::string s, int n) {
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

/// Fluent builder
struct Http {
  std::string method = "GET";
  std::string url;
  std::vector<std::pair<std::string, std::string>> headers;
  std::optional<std::string> body;
  int timeout_ms = 30'000;
};

/// Base setter returns the one that is used later for example for the
/// JsonRequestBuilder& -> D&
template <typename D>
class RequestBuilderBase {
protected:
  Http r_;
  D& self() { return static_cast<D&>(*this); };

  // clang-format off
 public:
  D& method(std::string m) { r_.method = std::move(m); return self(); }
  D& url(std::string u)    { r_.url = std::move(u);    return self(); };
  D& timeout(std::chrono::milliseconds ms) { r_.timeout_ms = static_cast<int>(ms.count()); return self(); };

  template <std::convertible_to<std::string_view> K, std::convertible_to<std::string_view> V>
  D& header(K&& k, V&& v) {
      r_.headers.emplace_back(std::string{std::string_view{k}}, std::string{std::string_view{v}});
      return self();
  }
  // clang-format on

  Http build() const { return r_; }
};

class RequestBuilder : public RequestBuilderBase<RequestBuilder> {};
class JsonRequestBuilder : public RequestBuilderBase<JsonRequestBuilder> {
public:
  JsonRequestBuilder& json(std::string body) {
    r_.body = std::move(body);
    return header("Content-Type", "application/json");
  }
};

/// Type state fluent builder -> only compiles when the required fields are set
struct User {
  std::string name;
  int age{0};
  std::string email;
};

/// If any of the required fields are missing it cant be built,
/// also cant rechain already set fields like UserBuilder{}.name().name()
template <bool hasName = false, bool hasAge = false>
class UserBuilder {
  User u_;
  // clang-format off
  template <bool, bool> friend class UserBuilder;
  // clang-format on
  UserBuilder(User u) : u_{std::move(u)} {}

public:
  // clang-format off
  UserBuilder() = default;
  auto name(std::string n) && requires(!hasName) { u_.name = std::move(n); 
      return UserBuilder<true, hasAge>{std::move(u_)}; }
  auto age(int a) && requires(!hasAge) { u_.age = a; return UserBuilder<hasName, true>{std::move(u_)}; }

  // Optional field
  UserBuilder&& email(std::string e) && { u_.email = std::move(e); return std::move(*this); };
  User build() && requires(hasName && hasAge) { return std::move(u_);}
  // clang-format on
};

// Detect finished builders
template <typename B>
concept CanBuild = requires(B&& b) { std::move(b).build(); };

/// Generic builder for any aggregate via pointers to members, directly
/// initializable without user defined constructors
template <typename T>
concept Buildable = std::is_aggregate_v<T> || std::default_initializable<T>;

template <Buildable T>
class Builder {
  T obj_{};

public:
  // clang-format off
  // it declares that M* is inside the scope of C
  template<typename C, typename M, typename V> requires std::same_as<C, T> && std::assignable_from<M&, V&&>
  Builder& with(M C::* member, V&& v) {
      obj_.*member = std::forward<V>(v);
      return *this;
  }

  template<std::invocable<T&> F>
  Builder& apply(F&& f) { std::invoke(std::forward<F>(f), obj_); }

  T build() const& { return obj_; }
  T build() && 	   { return std::move(obj_); }
  // clang-format on
};

/// Validated build using expected for c++23

struct Server {
  std::string host;
  int port;
};
enum class Err { NoHost, BadPort };

template <typename P>
concept Validateable = requires(const P& p) {
  { p.validate() } -> std::same_as<std::expected<void, Err>>;
};

class ServerBuilder {
  Server s_{"", 0};

public:
  // clang-format off
  ServerBuilder& host(std::string h) { s_.host = std::move(h); return *this; }
  ServerBuilder& port(int p) 	     { s_.port = p; 	       return *this; }
  // clang-format on

  std::expected<Server, Err> build() const {
    if (s_.host.empty()) return std::unexpected{Err::NoHost};
    if (s_.port <= 0 || s_.port > 65535) return std::unexpected{Err::BadPort};
    return s_;
  }
};
