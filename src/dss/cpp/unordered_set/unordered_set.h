#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>

#include "../unordered_multiset/unordered_multiset.h"
#include "../utils/utils.h"

namespace dss {
template <class Key, class Hash = std::hash<Key>,
          class KeyEqual = std::equal_to<Key>,
          class Allocator = std::allocator<Key>>
class unordered_set {
  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using hasher = Hash;
  using key_equal = KeyEqual;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<allocator_type>::pointer;
  using const_pointer = std::allocator_traits<allocator_type>::const_pointer;

  class Iterator {};
  class LocalIterator {};
  class NodeHandle {};

  template <typename Iter, typename NodeType>
  struct return_type {
    Iter position;
    bool inserted;
    NodeType node;
  };

  using iterator = Iterator;
  using const_iterator = const iterator;
  using local_iterator = LocalIterator;
  using const_local_iterator = const local_iterator;
  using node_type = NodeHandle;
  using insert_return_type = return_type<iterator, node_type>;

  unordered_set();
  explicit unordered_set(size_type bucket_count, const Hash& hash = Hash(),
                         const key_equal& equal = key_equal(),
                         const Allocator& alloc = Allocator());
  unordered_set(size_type bucket_count, const Allocator& alloc)
      : unordered_set(bucket_count, Hash(), key_equal(), alloc) {}
  unordered_set(size_type bucket_count, const Hash& hash,
                const Allocator& alloc)
      : unordered_set(bucket_count, hash, key_equal(), alloc) {}

  explicit unordered_set(const Allocator& alloc);
  template <std::input_iterator InputIt>
  unordered_set(InputIt first, InputIt last,
                size_type bucket_count = m_max_buckets,
                const Hash& hash = Hash(), const key_equal& equal = key_equal(),
                const Allocator& alloc = Allocator());

  template <std::input_iterator InputIt>
  unordered_set(InputIt first, InputIt last, size_type bucket_count,
                const Allocator& alloc)
      : unordered_set(first, last, bucket_count, Hash(), key_equal(), alloc) {}

  template <std::input_iterator InputIt>
  unordered_set(InputIt first, InputIt last, size_type bucket_count,
                const Hash& hash, const Allocator& alloc)
      : unordered_set(first, last, bucket_count, hash, key_equal(), alloc) {}

  unordered_set(const unordered_set& other);
  unordered_set(const unordered_set& other, const Allocator& alloc);
  unordered_set(unordered_set&& other);
  unordered_set(unordered_set&& other, const Allocator& alloc);

  unordered_set(std::initializer_list<value_type> init,
                size_type bucket_count = m_max_buckets,
                const Hash& hash = Hash(), const key_equal& equal = key_equal(),
                const Allocator& alloc = Allocator());

  unordered_set(std::initializer_list<value_type> init, size_type bucket_count,
                const Allocator& alloc)
      : unordered_set(init, bucket_count, Hash(), key_equal(), alloc) {}
  unordered_set(std::initializer_list<value_type> init, size_type bucket_count,
                const Hash& hash, const Allocator& alloc)
      : unordered_set(init, bucket_count, hash, key_equal(), alloc) {}

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L

  template <container_compatible_range<value_type> R>
  unordered_set(std::from_range_t, R&& rg,
                size_type bucket_count = m_max_buckets,
                const Hash& hash = Hash(), const key_equal& equal = key_equal(),
                const Allocator& alloc = Allocator());

  template <container_compatible_range<value_type> R>
  unordered_set(std::from_range_t, R&& rg, size_type bucket_count,
                const Allocator& alloc)
      : unordered_set(std::from_range, std::forward<R>(rg), bucket_count,
                      Hash(), key_equal(), alloc) {}
  template <container_compatible_range<value_type> R>
  unordered_set(std::from_range_t, R&& rg, size_type bucket_count,
                const Hash& hash, const Allocator& alloc)
      : unordered_set(std::from_range, std::forward<R>(rg), bucket_count, hash,
                      key_equal(), alloc) {}
  template <container_compatible_range<value_type> R>
  void insert_range(R&& rg);
#endif

  ~unordered_set();

  unordered_set& operator=(const unordered_set& other);
  unordered_set& operator=(unordered_set&& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_move_assignment::value);
  unordered_set& operator=(std::initializer_list<value_type> ilist);

  allocator_type get_allocator() const noexcept;

  // iterator
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  [[nodiscard]] bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  void clear() noexcept;

  // Insert
  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(value_type&& value);
  iterator insert(const_iterator hint, const value_type& value);
  iterator insert(const_iterator hint, value_type&& value);
  template <std::input_iterator InputIt>
  void insert(InputIt first, InputIt last);
  void insert(std::initializer_list<value_type> ilist);
  insert_return_type insert(node_type&& nh);
  iterator insert(const_iterator hint, node_type&& nh);

  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args);
  template <typename... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args);

  // erase
  iterator erase(iterator pos)
    requires(!std::same_as<iterator, const_iterator>);
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  size_type erase(const Key& key);
  template <typename K>
  size_type erase(K&& x);

  // extract
  node_type extract(const_iterator position);
  node_type extract(const Key& k);
  template <typename K>
  node_type extract(K&& x);

  // merge
  template <typename H2, typename P2>
  void merge(unordered_set<Key, H2, P2, Allocator>& source);
  template <typename H2, typename P2>
  void merge(unordered_set<Key, H2, P2, Allocator>&& source);
  template <typename H2, typename P2>
  void merge(unordered_multiset<Key, H2, P2, Allocator>& source);
  template <typename H2, typename P2>
  void merge(unordered_multiset<Key, H2, P2, Allocator>&& source);

  size_type count(const Key& key) const;
  template <typename K>
  size_type count(const K& x) const;

  // find
  iterator find(const Key& key);
  const_iterator find(const Key& key) const;
  template <typename K>
  iterator find(const K& x);
  template <typename K>
  const_iterator find(const K& x) const;

  bool contains(const Key& key) const;
  template <class K>
  bool contains(const K& x) const;

  // equal_range
  std::pair<iterator, iterator> equal_range(const Key& key);
  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;
  template <typename K>
  std::pair<iterator, iterator> equal_range(const K& x);
  template <typename K>
  std::pair<const_iterator, const_iterator> equal_range(const K& x) const;

  // iterators for local iterator
  local_iterator begin(size_type n);
  const_local_iterator begin(size_type n) const;
  const_local_iterator cbegin(size_type n) const;
  local_iterator end(size_type n);
  const_local_iterator end(size_type n) const;
  const_local_iterator cend(size_type n) const;

  size_type bucket_count() const;
  size_type max_bucket_count() const;
  size_type bucket_size(size_type n) const;
  size_type bucket(const Key& key) const;

  // hash policy
  float load_factor() const;
  float max_load_factor() const;
  void max_load_factor(float ml);
  void rehash(size_type count);

  void reserve(size_type count);

  // observers
  hasher hash_function() const;
  key_equal key_eq() const;

 private:
  static const size_type m_max_buckets = 16;
};

// Deduction guides
template <std::ranges::input_range R,
          class Hash = std::hash<std::ranges::range_value_t<R>>,
          class Pred = std::equal_to<std::ranges::range_value_t<R>>,
          class Alloc = std::allocator<std::ranges::range_value_t<R>>>
unordered_set(std::from_range_t, R&&, Hash = Hash(), Pred = Pred(),
              Alloc = Alloc())
    -> unordered_set<std::ranges::range_value_t<R>, Hash, Pred, Alloc>;

} // namespace dss

#endif // UNORDERED_SET_H
