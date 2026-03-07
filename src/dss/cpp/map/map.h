#ifndef MAP_H
#define MAP_H

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>

#include "../utils/utils.h"

namespace dss {

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class multimap {
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<allocator_type>::pointer;
  using const_pointer = std::allocator_traits<allocator_type>::const_pointer;

  // bidirectional iterator
  class Iterator {};
  class value_compare {};

  using iterator = Iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = const std::reverse_iterator<iterator>;
  using node_type = std::pair<Key, T>;

  multimap() : multimap(Compare()) {}
  explicit multimap(const Compare& comp, const Allocator& alloc = Allocator());
  explicit multimap(const Allocator& alloc);

  template <class InputIt>
  multimap(InputIt first, InputIt last, const Compare& comp = Compare(),
           const Allocator& alloc = Allocator());
  template <class InputIt>
  multimap(InputIt first, InputIt last, const Allocator& alloc)
      : multimap(first, last, Compare(), alloc) {}
  multimap(const multimap& other);
  multimap(const multimap& other, const Allocator& alloc);
  multimap(multimap&& other);
  multimap(multimap&& other, const Allocator& alloc);
  multimap(std::initializer_list<value_type> init,

           const Compare& comp = Compare(),
           const Allocator& alloc = Allocator());
  multimap(std::initializer_list<value_type> init, const Allocator& alloc)
      : multimap(init, Compare(), alloc) {}

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<value_type> R>
  multimap(std::from_range_t, R&& rg, const Compare& comp = Compare(),
           const Allocator& alloc = Allocator());
  template <container_compatible_range<value_type> R>
  multimap(std::from_range_t, R&& rg, const Allocator& alloc)
      : multimap(std::from_range, std::forward<R>(rg), Compare(), alloc) {}
#endif

  ~multimap();
  multimap& operator=(const multimap& other);
  multimap& operator=(multimap&& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_copy_assignment::value);
  multimap& operator=(std::initializer_list<value_type> ilist);
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

  iterator insert(const value_type& value);
  iterator insert(value_type&& value);
  template <class P>
  iterator insert(P&& value);
  iterator insert(const_iterator pos, const value_type& value);
  iterator insert(const_iterator pos, value_type&& value);
  template <class P>
  iterator insert(const_iterator pos, P&& value);
  template <class InputIt>
  void insert(InputIt first, InputIt last);
  void insert(std::initializer_list<value_type> ilist);
  iterator insert(node_type&& nh);
  iterator insert(const_iterator pos, node_type&& nh);

  template <container_compatible_range<value_type> R>
  void insert_range(R&& rg);
  template <class... Args>
  iterator emplace(Args&&... args);
  template <class... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  size_type erase(const Key& key);
  template <class K>
  size_type erase(K&& x);

  void swap(multimap& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_swap::value);

  node_type extract(const_iterator position);
  node_type extract(const Key& k);
  template <class K>
  node_type extract(K&& x);

  template <class C2>
  void merge(multimap<Key, T, C2, Allocator>& source);
  template <class C2>
  void merge(multimap<Key, T, C2, Allocator>&& source);

  size_type count(const Key& key) const;
  template <class K>
  size_type count(const K& x) const;

  const_iterator find(const Key& key) const;
  template <class K>
  iterator find(const K& x);
  template <class K>
  const_iterator find(const K& x) const;

  bool contains(const Key& key) const;
  template <class K>
  bool contains(const K& x) const;

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
  multimap::value_compare value_comp() const;
};

template <class Key, class T, class Compare, class Alloc>
constexpr auto operator<=>(const multimap<Key, T, Compare, Alloc>& lhs,
                           const multimap<Key, T, Compare, Alloc>& rhs);

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class map {
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<allocator_type>::pointer;
  using const_pointer = std::allocator_traits<allocator_type>::const_pointer;

  // bidirectional iterator
  class Iterator {};

  using iterator = Iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = const std::reverse_iterator<iterator>;

  using node_type = std::pair<Key, T>;

  template <typename Iter, typename NodeType>
  struct rty {};

  using insert_return_type = rty<iterator, node_type>;

  class value_compare {
   protected:
    Compare comp;

    value_compare(Compare c);
    bool operator()(const value_type& lhs, const value_type& rhs) const;
  };

  map() : map(Compare{}) {}
  explicit map(const Compare& comp, const Allocator& alloc = Allocator());
  explicit map(const Allocator& alloc);

  template <std::input_iterator InputIt>
  map(InputIt first, InputIt last, const Allocator& alloc)
      : map(first, last, Compare(), alloc) {}

  map(const map& other);
  map(const map& other, const Allocator& alloc);
  map(map&& other);
  map(map&& other, const Allocator& alloc);

  map(std::initializer_list<value_type> init, const Allocator& alloc)
      : map(init, Compare(), alloc) {}

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<value_type> R>
  map(std::from_range_t, R&& rg, const Compare& comp = Compare(),
      const Allocator& alloc = Allocator());

  template <container_compatible_range<value_type> R>
  map(std::from_range_t, R&& rg, const Allocator& alloc)
      : map(std::from_range, std::forward<R>(rg), Compare(), alloc) {}
  template <container_compatible_range<value_type> R>
  void insert_range(R&& rg);

#endif

  ~map();

  // operators
  map& operator=(const map& other);
  map& operator=(map&& other) noexcept(
      std::allocator_traits<allocator_type>::is_always_equal::value);
  map& operator=(std::initializer_list<value_type> ilist);

  allocator_type get_allocator() const noexcept;
  const T& at(const Key& key) const;

  T& operator[](Key&& key);

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

  template <typename P>
  std::pair<iterator, bool> insert(P&& value);

  std::pair<iterator, bool> insert(value_type&& value);
  iterator insert(const_iterator pos, const value_type& value);
  template <class P>
  iterator insert(const_iterator pos, P&& value);
  iterator insert(const_iterator pos, value_type&& value);

  template <std::input_iterator InputIt>
  void insert(InputIt first, InputIt last);
  void insert(std::initializer_list<value_type> ilist);
  insert_return_type insert(node_type&& nh);
  iterator insert(const_iterator pos, node_type&& nh);

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

  // try emplacing element passed at the position, using the hint
  template <class... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args);
  template <class... Args>
  std::pair<iterator, bool> try_emplace(Key&& k, Args&&... args);
  template <class... Args>
  iterator try_emplace(const_iterator hint, const Key& k, Args&&... args);
  template <class... Args>
  iterator try_emplace(const_iterator hint, Key&& k, Args&&... args);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  size_type erase(const Key& key);

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L

  template <class K>
  size_type erase(K&& x);

  template <class K>
  node_type extract(K&& x);
#endif

  void swap(map& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_swap::value);

  node_type extract(const Key& k);
  node_type extract(const_iterator position);

  // merging
  template <class C2>
  void merge(map<Key, T, C2, Allocator>& source);
  template <class C2>
  void merge(map<Key, T, C2, Allocator>&& source);
  template <class C2>
  void merge(dss::multimap<Key, T, C2, Allocator>& source);
  template <class C2>
  void merge(dss::multimap<Key, T, C2, Allocator>&& source);

  size_type count(const Key& key) const;
  template <class K>
  size_type count(const K& x) const;

  iterator find(const Key& key);
  const_iterator find(const Key& key) const;
  template <class K>
  iterator find(const K& x);
  template <class K>
  const_iterator find(const K& x) const;

  // finally added the contains
  bool contains(const Key& key) const;
  template <class K>
  bool contains(const K& x) const;

  template <class K>
  std::pair<iterator, iterator> equal_range(const K& x);
  template <class K>
  std::pair<const_iterator, const_iterator> equal_range(const K& x) const;

  // upper and lower bounds
  iterator lower_bound(const Key& key);
  const_iterator lower_bound(const Key& key) const;
  template <class K>
  iterator lower_bound(const K& x);
  iterator upper_bound(const Key& key);
  const_iterator upper_bound(const Key& key) const;
  template <class K>
  iterator upper_bound(const K& x);
  template <class K>
  const_iterator upper_bound(const K& x) const;

  key_compare key_comp() const;
  map::value_compare value_comp() const;
};

template <class Key, class T, class Compare, class Alloc>
constexpr auto operator<=>(const map<Key, T, Compare, Alloc>& lhs,
                           const map<Key, T, Compare, Alloc>& rhs);

} // namespace dss

#endif // MAP_H
