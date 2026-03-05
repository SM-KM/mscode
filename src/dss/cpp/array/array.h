#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <iterator>
#include <type_traits>

template <class T, size_t N>
struct array {
  using value_type = T;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const pointer;
  using iterator = std::contiguous_iterator_tag;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = const reverse_iterator;

  // constructor - destructor and operator = are normally implicitly declared
  // inside the standard library
  constexpr array();
  constexpr ~array();
  void operator=(const array&) = default;

  constexpr reference at(size_type pos);
  constexpr const_reference at(size_type pos) const;
  constexpr reference operator[](size_type pos);
  constexpr const_reference operator[](size_type pos) const;

  constexpr reference front();
  constexpr const_reference front() const;
  constexpr reference back();
  constexpr const_reference back() const;
  constexpr T* data() noexcept;
  constexpr const T* data() const;

  // iterators
  constexpr iterator begin() noexcept;
  constexpr const_iterator begin() const noexcept;
  constexpr const_iterator cbegin() const noexcept;
  constexpr iterator end() noexcept;
  constexpr const_iterator end() const noexcept;
  constexpr const_iterator cend() const noexcept;
  constexpr iterator rbegin() noexcept;
  constexpr const_iterator rbegin() const noexcept;
  constexpr const_iterator crbegin() const noexcept;
  constexpr iterator rend() noexcept;
  constexpr const_iterator rend() const noexcept;
  constexpr const_iterator crend() const noexcept;

  [[nodiscard]] constexpr bool empty() const noexcept;
  [[nodiscard]] constexpr size_type size() const noexcept;
  constexpr size_type max_size() const noexcept;
  void swap(array& other) noexcept(std::is_nothrow_swappable_v<T>);
  void fill(const T& value);

  // template <typename U, std::size_t S>
  // constexpr bool operator<=>(const std::array<T, N>& lhs,
  //                            const std::array<T, N>& rhs);
};

template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]);
template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&&a)[N]);

#endif // ARRAY_H
