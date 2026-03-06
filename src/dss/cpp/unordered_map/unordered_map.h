#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H

#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

namespace dss {
template <class Key, class T, class Hash = std::hash<Key>,
          class KeyEqual = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map {
  // iterator invalidation cases
  // always: clear, rehash, reserve, operator=, and for emplacing and
  // inserting also operator[] only if it causes a rehash

  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using hasher = Hash;
  using key_equal = KeyEqual;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<allocator_type>::pointer;
  using const_pointer = std::allocator_traits<allocator_type>::const_pointer;

  // forward iterators
  class Iterator {};
  class LocalIterator {}; // use to iterate on the current bucket

  template <typename K, typename V>
  class NodeHandle {};

  using iterator = Iterator;
  using const_iterator = const Iterator;
  using local_iterator = LocalIterator;
  using const_local_iterator = const LocalIterator;
  using node_type = NodeHandle<key_type, value_type>;

  unordered_map();
};

} // namespace dss
#endif // UNORDEREDMAP_H
