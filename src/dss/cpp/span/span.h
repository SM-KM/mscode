#ifndef SPAN_H
#define SPAN_H

#include <cstddef>
#include <iterator>
#include <limits>
#include <span>
#include <type_traits>

namespace dss {
template <class T, size_t Extent = std::dynamic_extent>
class span {
  using element_type = T;
  using value_type = std::remove_cv<T>;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const pointer;
  using reference = T&;
  using const_reference = const T&;

  // contigious iterator
  class Iterator {};

  using iterator = Iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = const reverse_iterator;
  static constexpr std::size_t extent = Extent;

  constexpr span() noexcept;
  template <class It>
  explicit(extent != std::dynamic_extent) constexpr span(It first,
                                                         size_type count);

  template <class It, class End>
  explicit(extent != std::dynamic_extent) constexpr span(It first, End last);
  template <std::size_t N>
  constexpr span(std::type_identity_t<element_type> (&arr)[N]) noexcept;
  template <class U, std::size_t N>
  constexpr span(std::array<U, N>& arr) noexcept;
  template <class U, std::size_t N>
  constexpr span(const std::array<U, N>& arr) noexcept;
  template <class R>
  explicit(extent != std::dynamic_extent) constexpr span(R&& range);
  explicit(extent != std::dynamic_extent) constexpr span(
      std::initializer_list<value_type> il) noexcept;

  template <class U, std::size_t N>
  explicit(extent != std::dynamic_extent &&
           N == std::dynamic_extent) constexpr span(const std::span<U, N>&
                                                        source) noexcept;
  constexpr span(const span& other) noexcept = default;

  constexpr span& operator=(const span& other) noexcept = default;
  ~span() = default;

  // iterators
  constexpr iterator begin() const noexcept;
  constexpr const_iterator cbegin() const noexcept;
  constexpr iterator end() const noexcept;
  constexpr const_iterator cend() const noexcept;
  constexpr reverse_iterator rbegin() const noexcept;
  constexpr const_reverse_iterator crbegin() const noexcept;
  constexpr reverse_iterator rend() const noexcept;
  constexpr const_reverse_iterator crend() const noexcept;

  constexpr reference front() const;
  constexpr reference back() const;
  constexpr reference at(size_type pos) const;
  constexpr reference operator[](size_type idx) const;

  constexpr pointer data() const noexcept;
  constexpr size_type size() const noexcept;
  constexpr size_type size_bytes() const noexcept;

  [[nodiscard]] constexpr bool empty() const noexcept;

  template <std::size_t Count>
  constexpr std::span<element_type, Count> first() const;
  constexpr std::span<element_type, std::dynamic_extent> first(
      size_type Count) const;

  template <std::size_t Count>
  constexpr std::span<element_type, Count> last() const;
  constexpr std::span<element_type, std::dynamic_extent> last(
      size_type Count) const;

  template <std::size_t Offset, std::size_t Count = std::dynamic_extent>
  constexpr std::span<element_type, std::dynamic_extent> subspan() const;
};

template <class T, std::size_t N>
std::span<const std::byte, std::dynamic_extent> as_bytes(
    std::span<T, N> s) noexcept;
template <class T, std::size_t N>
std::span<std::byte, std::dynamic_extent> as_writable_bytes(
    std::span<T, N> s) noexcept;

inline constexpr std::size_t dynamic_extent =
    std::numeric_limits<std::size_t>::max();
} // namespace dss

#endif // SPAN_H
