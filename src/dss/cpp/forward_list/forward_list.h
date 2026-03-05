#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <cstddef>
#include <iterator>
#include <memory>

#include "../utils/utils.h"

namespace dss {

template <typename T, typename Allocator = std::allocator<T>>
class forward_list {
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = std::allocator_traits<allocator_type>::pointer;
  using const_pointer = std::allocator_traits<allocator_type>::const_pointer;

  // forward iterator of value type
  class Iterator {};

  using iterator = Iterator;
  using const_iterator = const iterator;

  forward_list();
  explicit forward_list(const Allocator& alloc);
  forward_list(size_type count, const T& value,
               const Allocator& alloc = Allocator());
  explicit forward_list(size_type count);
  explicit forward_list(size_type count, const Allocator& alloc = Allocator());

  forward_list(const forward_list& other);
  forward_list(const forward_list& other, const Allocator& alloc);
  forward_list(forward_list&& other);
  forward_list(forward_list&& other, const Allocator& alloc);
  forward_list(std::initializer_list<T> init,
               const Allocator& alloc = Allocator());

  template <std::input_iterator InputIt>
  forward_list(InputIt first, InputIt last,
               const Allocator& alloc = Allocator());

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<T> R>
  forward_list(std::from_range_t, R&& rg, const Allocator& alloc = Allocator());

  template <container_compatible_range<T> R>
  void assign_range(R&& rg);

  template <container_compatible_range<T> R>
  iterator insert_range_after(const_iterator pos, R&& rg);

  template <container_compatible_range<T> R>
  void prepend_range(R&& rg);
#endif

  ~forward_list();

  forward_list& operator=(const forward_list& other);
  forward_list& operator=(forward_list&& other) noexcept(
      std::allocator_traits<
          allocator_type>::propagate_on_container_move_assignment::value);
  forward_list& operator=(std::initializer_list<T> ilist);

  void assign(size_type count, const T& value);
  template <std::input_iterator InputIt>
  void assign(InputIt first, InputIt last);
  void assign(std::initializer_list<T> ilist);

  allocator_type get_allocator() const noexcept { return m_allocator; };

  reference front();
  const_reference front() const;

  iterator before_begin() noexcept;
  const_iterator before_begin() const noexcept;
  const_iterator cbefore_begin() const noexcept;
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  [[nodiscard]] bool empty() const noexcept;
  size_type max_size() const noexcept;

  void clear() noexcept;

  iterator insert_after(const_iterator pos, const T& value);
  iterator insert_after(const_iterator pos, T&& value);
  iterator insert_after(const_iterator pos, size_type count, const T& value);
  template <std::input_iterator InputIt>
  iterator insert_after(const_iterator pos, InputIt first, InputIt last);
  iterator insert_after(const_iterator pos, std::initializer_list<T> ilist);

  template <class... Args>
  iterator emplace_after(const_iterator pos, Args&&... args);

  iterator erase_after(const_iterator pos);
  iterator erase_after(const_iterator first, const_iterator last);

  void push_front(T&& value);
  void pop_front();

  template <class... Args>
  reference emplace_front(Args&&... args);

  void resize(size_type count);
  void resize(size_type count, const value_type& value);

  void swap(forward_list& other) noexcept(
      std::allocator_traits<allocator_type>::is_always_equal::value);

  void merge(forward_list& other);
  void merge(forward_list&& other);
  template <typename Compare>
  void merge(forward_list& other, Compare comp);
  template <typename Compare>
  void merge(forward_list&& other, Compare comp);

  void splice_after(const_iterator pos, forward_list& other);
  void splice_after(const_iterator pos, forward_list&& other);
  void splice_after(const_iterator pos, forward_list& other, const_iterator it);
  void splice_after(const_iterator pos, forward_list&& other,
                    const_iterator it);
  void splice_after(const_iterator pos, forward_list& other,
                    const_iterator first, const_iterator last);
  void splice_after(const_iterator pos, forward_list&& other,
                    const_iterator first, const_iterator last);

  size_type remove(const T& value);
  template <typename UnaryPredicate>
  size_type remove_if(UnaryPredicate p);
  void reverse() noexcept;

  size_type unique();
  template <typename BinaryPredicate>
  size_type unique(BinaryPredicate p);

  void sort();
  template <class Compare>
  void sort(Compare comp);

 private:
  allocator_type m_allocator = Allocator{};
};

template <class T, class Alloc>
constexpr auto operator<=>(const forward_list<T, Alloc>& lhs,
                           const forward_list<T, Alloc>& rhs);

template <std::ranges::input_range R,
          class Alloc = std::allocator<std::ranges::range_value_t<R>>>
forward_list(std::from_range_t, R&&, Alloc = Alloc())
    -> forward_list<std::ranges::range_value_t<R>, Alloc>;

} // namespace dss

#endif // SINGLELINKEDLIST_H
