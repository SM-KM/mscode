#ifndef DEQUE_H
#define DEQUE_H

#include <compare>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>

#include "../utils/utils.h"

namespace dss {

// iterator invalidation cases
// swap, std::swap, shrink_to_fit, clear, insert, emplace, push_front,
// puch_back, emplcae_front, emplace_back, erase, resize, pop_front, po_back

template <class T, class Allocator = std::allocator<T>>
class deque {
  // random_access iterator
  class Iterator {};

  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<Allocator>::pointer;
  using const_pointer = std::allocator_traits<Allocator>::const_pointer;
  using iterator_type = std::random_access_iterator_tag;
  using iterator = Iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<value_type>;
  using const_reverse_iterator = const reverse_iterator;

  deque();
  explicit deque(const Allocator& alloc);
  deque(size_type count, const T& value, const Allocator& alloc = Allocator());
  explicit deque(size_type count, const Allocator& alloc = Allocator());

  template <class InputIt>
  deque(InputIt first, InputIt last, const Allocator& alloc = Allocator());

  deque(const deque& other);
  deque(const deque& other, const Allocator& alloc);

  deque(deque&& other);
  deque(deque&& other, const Allocator& alloc);
  deque(std::initializer_list<T> init, const Allocator& alloc = Allocator());
  template <container_compatible_range<T> R>
  deque(std::from_range_t, R&& rg, const Allocator& alloc = Allocator());

  ~deque();

  deque& operator=(const deque& other);
  deque& operator=(deque&& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_move_assignment::value);
  deque& operator=(std::initializer_list<T> ilist);

  void assign(size_type count, const T& value);
  template <std::input_iterator InputIt>
  void assign(InputIt first, InputIt last);
  void assign(std::initializer_list<T> ilist);

  template <container_compatible_range<T> R>
  void assign_range(R&& rg);

  allocator_type get_allocator() const noexcept;

  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

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

  void shrink_to_fit();
  void clear() noexcept;

  iterator insert(const_iterator pos, const T& value);
  iterator insert(const_iterator pos, T&& value);
  iterator insert(const_iterator pos, size_type count, const T& value);
  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last);
  iterator insert(const_iterator pos, std::initializer_list<T> ilist);

  // validate ranges is supported with the additions of c++23
#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<T> R>
  iterator insert_range(const_iterator pos, R&& rg);
  template <container_compatible_range<T> R>
  void append_range(R&& rg);
  template <container_compatible_range<T> R>
  void prepend_range(R&& rg);
#endif // __cplusplus >= 202302L

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void push_back(T&& value);
  void pop_back();
  void pop_front();
  void push_front(T&& value);

  template <class... Args>
  reference emplace_back(Args&&... args);
  template <class... Args>
  reference emplace_front(Args&&... args);

  void resize(size_type count);
  void resize(size_type count, const value_type& value);

  void swap(deque& other) noexcept(
      std::allocator_traits<allocator_type>::is_always_equal::value);
};

template <class T, class Alloc>
constexpr auto operator<=>(const deque<T, Alloc>& lhs,
                           const deque<T, Alloc>& rhs);

} // namespace dss

// deduction guide for ranges
// template <std::ranges::input_range R,
//           class Alloc = std::allocator<std::ranges::range_value_t<R>>>
// dss::deque(std::from_range_t, R&&, Alloc = Alloc())
//     ->dss::deque<std::ranges::range_value_t<R>, Alloc>;

#endif // DEQUE_H
