#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H

#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

namespace dss {
template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class UnorderedMap {
public:
  struct iterator {
    T m_ptr;
  };

  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using hasher = Hash;
  using key_equal = Pred;
  using allocator_type = Allocator;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  using iterator = std::nullptr_t;
  using const_iterator = std::nullptr_t;
  using local_iterator =
      std::nullptr_t; // is used for iterating in a single bucket

  // using node_type <- search this
  // using insert_return_type =  <- also search this

  UnorderedMap();
};

template <typename Key, typename T, typename Hash, typename Pred,
          typename Alloc>
bool operator==(const UnorderedMap<Key, T, Hash, Pred, Alloc>& a,
                const UnorderedMap<Key, T, Hash, Pred, Alloc>& b);

} // namespace dss
#endif // UNORDEREDMAP_H
