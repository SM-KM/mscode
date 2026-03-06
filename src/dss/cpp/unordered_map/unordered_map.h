#ifndef UNORDEREDMAP_H
#define UNORDEREDMAP_H

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>

#include "../utils/utils.h"

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
  explicit unordered_map(size_type bucket_count, const Hash& hash = Hash(),
                         const key_equal& equal = key_equal(),
                         const Allocator& alloc = Allocator());
  unordered_map(size_type bucket_count, const Allocator& alloc)
      : unordered_map(bucket_count, Hash(), key_equal(), alloc) {}
  unordered_map(size_type bucket_count, const Hash& hash,
                const Allocator& alloc)
      : unordered_map(bucket_count, hash, key_equal(), alloc) {}
  explicit unordered_map(const Allocator& alloc);
  template <class InputIt>
  unordered_map(InputIt first, InputIt last,
                size_type bucket_count = m_min_buckets,
                const Hash& hash = Hash(), const key_equal& equal = key_equal(),
                const Allocator& alloc = Allocator());

  template <typename InputIt>
  unordered_map(InputIt first, InputIt last, size_type bucket_count,
                const Allocator& alloc)
      : unordered_map(first, last, bucket_count, Hash(), key_equal(), alloc) {}

  template <typename InputIt>
  unordered_map(InputIt first, InputIt last, size_type bucket_count,
                const Hash& hash, const Allocator& alloc)
      : unordered_map(first, last, bucket_count, hash, key_equal(), alloc) {}

  unordered_map(const unordered_map& other);
  unordered_map(const unordered_map& other, const Allocator& alloc);
  unordered_map(unordered_map&& other);
  unordered_map(unordered_map&& other, const Allocator& alloc);

  // maybe remove this one and always have to define the bucket count
  unordered_map(std::initializer_list<value_type> init,
                size_type bucket_count = m_min_buckets,
                const Hash& hash = Hash(), const key_equal& equal = key_equal(),
                const Allocator& alloc = Allocator());

  explicit unordered_map(std::initializer_list<value_type> init,
                         size_type bucket_count, const Allocator& alloc)
      : unordered_map(init, bucket_count, Hash(), key_equal(), alloc) {}

  unordered_map(std::initializer_list<value_type> init, size_type bucket_count,
                const Hash& hash, const Allocator& alloc)
      : unordered_map(init, bucket_count, hash, key_equal(), alloc) {}

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<value_type> R>
  unordered_map(std::from_range_t, R&& rg,
                size_type bucket_count = m_min_buckets,
                const Hash& hash = Hash(), const key_equal& equal = key_equal(),
                const Allocator& alloc = Allocator());

  template <container_compatible_range<value_type> R>
  unordered_map(std::from_range_t, R&& rg, size_type bucket_count,
                const Allocator& alloc)
      : unordered_map(std::from_range, std::forward<R>(rg), bucket_count,
                      Hash(), key_equal(), alloc) {}

  template <container_compatible_range<value_type> R>
  unordered_map(std::from_range_t, R&& rg, size_type bucket_count,
                const Hash& hash, const Allocator& alloc)
      : unordered_map(std::from_range, std::forward<R>(rg), bucket_count, hash,
                      key_equal(), alloc) {}

  template <container_compatible_range<value_type> R>
  void insert_range(R&& rg);
#endif

  ~unordered_map();

  unordered_map& operator=(const unordered_map& other);
  unordered_map& operator=(unordered_map&& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_move_assignment::value);
  unordered_map& operator=(std::initializer_list<value_type> ilist);
  allocator_type get_allocator() const noexcept;

  // iterators
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  [[nodiscard]] bool empty() const noexcept;
  size_type size() const noexcept { return m_size; };
  size_type max_size() const noexcept;
  void clear() noexcept;

  // insertion
  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(value_type&& value);

  template <typename P>
    requires std::is_constructible_v<value_type, P&&>
  std::pair<iterator, bool> insert(P&& value);

  iterator insert(const_iterator hint, const value_type& value);
  iterator insert(const_iterator hint, value_type&& value);

  template <typename P>
    requires std::is_constructible_v<value_type, P&&>
  iterator insert(const_iterator hint, P&& value);

  template <std::input_iterator InputIt>
  void insert(InputIt first, InputIt last);
  void insert(std::initializer_list<value_type> ilist);
  iterator insert(const_iterator hint, node_type&& nh);

  template <class M>
  std::pair<iterator, bool> insert_or_assign(const Key& k, M&& obj);
  template <class M>
  std::pair<iterator, bool> insert_or_assign(Key&& k, M&& obj);
  template <class M>
  iterator insert_or_assign(const_iterator hint, const Key& k, M&& obj);
  template <class M>
  iterator insert_or_assign(const_iterator hint, Key&& k, M&& obj);

  template <class... Args>
  std::pair<iterator, bool> emplace(Args&&... args);

 private:
  static const size_t m_min_buckets = 16;
  size_type m_size;
};

} // namespace dss
#endif // UNORDEREDMAP_H
