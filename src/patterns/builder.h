#pragma once
// INFO: change the dss from the standard with the ones
// implemented in dss

#include <chrono>
#include <concepts>
#include <optional>
#include <string>
#include <string_view>
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

/// Generic builder for any struct
/// Validated build using expected for c++23
