#ifndef SET_H
#define SET_H

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>

#include "../utils/utils.h"

namespace dss {
template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key>>
class set {
  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using value_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = Allocator::pointer;
  using const_pointer = Allocator::const_pointer;

  class Iterator {};
  class NodeHandle {};

  using iterator = Iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<Iterator>;
  using const_reverse_iterator = const std::reverse_iterator<Iterator>;
  using node_type = NodeHandle;
};

template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key>>
class multiset {
  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using value_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = Allocator::pointer;
  using const_pointer = Allocator::const_pointer;

  class Iterator {};
  class NodeHandle {};

  using iterator = Iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<Iterator>;
  using const_reverse_iterator = const std::reverse_iterator<Iterator>;
  using node_type = NodeHandle;

  multiset() : multiset(Compare()) {}
  explicit multiset(const Compare& comp, const Allocator& alloc = Allocator());
  explicit multiset(const Allocator& alloc);
  template <class InputIt>
  multiset(InputIt first, InputIt last, const Compare& comp = Compare(),
           const Allocator& alloc = Allocator());
  template <class InputIt>
  multiset(InputIt first, InputIt last, const Allocator& alloc)
      : multiset(first, last, Compare(), alloc) {}
  multiset(const multiset& other);

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<value_type> R>
  multiset(std::from_range_t, R&& rg, const Compare& comp = Compare(),
           const Allocator& alloc = Allocator());
  template <container_compatible_range<value_type> R>
  multiset(std::from_range_t, R&& rg, const Allocator& alloc)
      : multiset(std::from_range, std::forward<R>(rg), Compare(), alloc) {}

  template <container_compatible_range<value_type> R>
  void insert_range(R&& rg);
#endif // 0

  ~multiset();
  multiset& operator=(const multiset& other);
  multiset& operator=(multiset&& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_copy_assignment::value);
  multiset& operator=(std::initializer_list<value_type> ilist);

  allocator_type get_allocator() const noexcept;

  // iterators
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;

  [[nodiscard]] bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void clear() noexcept;

  // insert
  iterator insert(const value_type& value);
  iterator insert(value_type&& value);
  iterator insert(const_iterator pos, const value_type& value);
  iterator insert(const_iterator pos, value_type&& value);
  template <class InputIt>
  void insert(InputIt first, InputIt last);
  void insert(std::initializer_list<value_type> ilist);
  iterator insert(node_type&& nh);
  iterator insert(const_iterator pos, node_type&& nh);

  template <class... Args>
  iterator emplace(Args&&... args);
  template <class... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args);

  iterator erase(const_iterator pos);
  iterator erase(iterator pos)
    requires(!std::same_as<iterator, const_iterator>);
  iterator erase(const_iterator first, const_iterator last);
  size_type erase(const Key& key);
  template <class K>
  size_type erase(K&& x);

  void swap(multiset& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_swap::value);

  node_type extract(const_iterator position);
  node_type extract(const Key& k);
  template <class K>
  node_type extract(K&& x);

  template <class C2>
  void merge(multiset<Key, C2, Allocator>& source);
  template <class C2>
  void merge(multiset<Key, C2, Allocator>&& source);

  size_type count(const Key& key) const;
  template <class K>
  size_type count(const K& x) const;

  iterator find(const Key& key);
  const_iterator find(const Key& key) const;
  template <class K>
  iterator find(const K& x);
  template <class K>
  const_iterator find(const K& x) const;

  bool contains(const Key& key) const;
  template <class K>
  bool contains(const K& x) const;

  std::pair<iterator, iterator> equal_range(const Key& key);
  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;
  template <class K>
  std::pair<iterator, iterator> equal_range(const K& x);
  template <class K>
  std::pair<const_iterator, const_iterator> equal_range(const K& x) const;

  iterator lower_bound(const Key& key);
  const_iterator lower_bound(const Key& key) const;
  template <class K>
  iterator lower_bound(const K& x);
  template <class K>
  const_iterator lower_bound(const K& x) const;

  iterator upper_bound(const Key& key);
  const_iterator upper_bound(const Key& key) const;
  template <class K>
  iterator upper_bound(const K& x);
  template <class K>
  const_iterator upper_bound(const K& x) const;

  key_compare key_comp() const;
  multiset::value_compare value_comp() const;
};

template <class Key, class Compare, class Alloc>
constexpr auto operator<=>(const multiset<Key, Compare, Alloc>& lhs,
                           const multiset<Key, Compare, Alloc>& rhs);

} // namespace dss

#endif // SET_H
