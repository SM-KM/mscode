#pragma once

#include <concepts>
#include <cstddef>
#include <memory>
#include <ostream>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

/// Composite lets u treat a single object and a group of objects
/// uniformely (tree).
struct Vec2 {
  float x{0}, y{0};
  friend Vec2 operator+(Vec2 a, Vec2 b) { return {a.x + b.x, a.y + b.y}; };
};

/// Dynamic composite
class Node {
  std::string name_;
  Vec2 local_;

public:
  Node(std::string name, Vec2 local) : name_{name}, local_{local} {}
  Node(const Node &) = delete;
  Node &operator=(const Node &) = delete;
  virtual ~Node() = default;

  const std::string &name() const { return name_; }
  Vec2 local() const { return local_; }

  virtual std::size_t triangles() const = 0;
  virtual void draw(std::ostream &os, Vec2 parent_world, int depth) const = 0;
  virtual std::span<const std::unique_ptr<Node>> children() const { return {}; }
};

class Sprite final : public Node {
public:
  using Node::Node;
  std::size_t triangles() const override { return 2; }
  void draw(std::ostream &os, Vec2 p, int d) const override {};
};

class Mesh final : public Node {
  std::size_t tris_;

public:
  Mesh(std::string name, Vec2 local, std::size_t tris)
      : Node{std::move(name), local}, tris_{tris} {};
  std::size_t triangles() const override { return tris_; }
  void draw(std::ostream &os, Vec2 p, int d) const override {};
};

class Group final : public Node {
  std::vector<std::unique_ptr<Node>> kids_;
  // clang-format off

public:
  using Node::Node;
  // Build children in place, returns a reference so the tree can be built freely
  template <std::derived_from<Node> T, typename... Args> requires std::constructible_from<T, Args...>
  T &add(Args &&...args) {
    auto p = std::make_unique<T>(std::forward<Args>(args)...);
    T &ref = *p;
    kids_.push_back(std::move(p));
    return ref;
  };

  std::size_t triangles() const override {
      std::size_t t{0};
      for(auto& k : kids_) t += k->triangles();
      return t;
  }
  void draw(std::ostream &os, Vec2 p, int d) const override {};
  std::span<const std::unique_ptr<Node>> children() const override { return kids_; }
  // clang-format on
};

// clang-format off
/// Generic traversal -> works for every node with the children()
template <typename F> requires std::invocable<F &, const Node &, int>
void walk(const Node &n, F &&f, int depth = 0) {
    f(n, depth);
    for(const auto& c : n.children()) walk(*c, f, depth + 1);
}

inline const Node* find(const Node& root, std::string_view name) {
    const Node* found = nullptr;
    walk(root, [&](const Node& n, int){ if (!found && n.name() == name) found = &n; });
    return found;
}


/// Variant composite: a closed set of node types, no virtuals
struct Sprt { std::string name; std::size_t tris = 2; };
struct Light { std::string name; float intensity = 1; };

template<typename T>
concept LeafNode = requires (const T& t) {
    { t.name } -> std::convertible_to<const std::string&>; 
    requires std::same_as<T, Sprt> || std::same_as<T, Light>;
};

struct Nd;
struct Grp {
    std::string name;
    std::vector<Nd> kids;
    template<LeafNode L> Grp& add(L leaf);
    Grp& add(Grp g);
};

struct Nd : std::variant<Sprt, Light, Grp> {
    using variant::variant;
    const std::variant<Sprt, Light, Grp>& v() const { return *this; }
};

// clang-format off
template <LeafNode L> Grp &Grp::add(L leaf) { kids.emplace_back(std::move(leaf)); return *this; }
inline Grp& Grp::add(Grp g) { kids.emplace_back(std::move(g)); return *this; }

template <typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// clang-format on
