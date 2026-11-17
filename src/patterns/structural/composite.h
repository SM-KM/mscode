#pragma once

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
};
