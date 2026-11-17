#pragma once

#include <memory>
#include <ostream>
#include <span>
#include <string>

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
