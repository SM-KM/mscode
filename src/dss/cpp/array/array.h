#ifndef ARRAY_H
#define ARRAY_H

#include <algorithm>
#include <compare>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <type_traits>

namespace dss {

namespace detail {
template <typename T, std::size_t N>
struct array_storage {
  using type = T[N];
};

template <typename T>
struct array_storage<T, 0> {
  struct type {};
};
} // namespace detail

template <typename T, size_t N>
class array {
 public:
  using value_type = T;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const pointer;
  using iterator = value_type*;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = const reverse_iterator;

  typename detail::array_storage<T, N>::type m_elems;

  // constructor - destructor and operator = are normally implicitly declared
  // inside the standard library
  constexpr array() = default;
  constexpr ~array() = default;
  void operator=(const array&) = default;

  constexpr reference at(size_type pos) {
    if (pos >= N) throw std::out_of_range("dss::array::at");
    return data()[pos];
  };
  constexpr const_reference at(size_type pos) const {
    if (pos >= N) throw std::out_of_range("dss::array::at");
    return data()[pos];
  };

  constexpr reference operator[](size_type pos) { return data()[pos]; };
  constexpr const_reference operator[](size_type pos) const {
    return data()[pos];
  };

  constexpr reference front() { return data()[0]; };
  constexpr const_reference front() const { return data()[0]; };
  constexpr reference back() { return data()[N - 1]; };
  constexpr const_reference back() const { return data()[N - 1]; };
  constexpr T* data() noexcept {
    if constexpr (N == 0) {
      return nullptr;
    } else {
      return m_elems;
    }
  };
  constexpr const T* data() const noexcept {
    if constexpr (N == 0) {
      return nullptr;
    } else {
      return m_elems;
    }
  };

  // iterators
  constexpr iterator begin() noexcept { return data(); };
  constexpr const_iterator begin() const noexcept { return data(); };
  constexpr const_iterator cbegin() const noexcept { return data(); };
  constexpr iterator end() noexcept { return data() + N; };
  constexpr const_iterator end() const noexcept { return data() + N; };
  constexpr const_iterator cend() const noexcept { return data() + N; };

  constexpr iterator rbegin() noexcept { return reverse_iterator{end()}; };
  constexpr const_iterator rbegin() const noexcept {
    return const_reverse_iterator{end()};
  };
  constexpr const_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  };
  constexpr iterator rend() noexcept { return reverse_iterator{begin()}; };
  constexpr const_iterator rend() const noexcept {
    return const_reverse_iterator{begin()};
  };
  constexpr const_iterator crend() const noexcept {
    return const_reverse_iterator{begin()};
  };

  [[nodiscard]] constexpr bool empty() const noexcept { return N == 0; };
  [[nodiscard]] constexpr size_type size() const noexcept { return N; };
  constexpr size_type max_size() const noexcept { return N; };

  void swap(array& other) noexcept(std::is_nothrow_swappable_v<T>) {
    std::swap_ranges(begin(), end(), other.begin());
  };
  void fill(const T& value) { std::fill_n(begin(), N, value); };
};

template <typename T, std::size_t N>
constexpr bool operator==(const array<T, N>& lhs, const array<T, N>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, std::size_t N>
  requires std::three_way_comparable<T>
constexpr auto operator<=>(const array<T, N>& lhs, const array<T, N>& rhs) {
  return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
};

template <std::size_t I, class T, std::size_t N>
constexpr T& get(array<T, N>& a) noexcept {
  static_assert(I < N, "array index out of bounds");
  return a.m_elems[I];
}
template <std::size_t I, class T, std::size_t N>
constexpr const T& get(const array<T, N>& a) noexcept {
  static_assert(I < N, "array index out of bounds");
  return a.m_elems[I];
}
template <std::size_t I, class T, std::size_t N>
constexpr T&& get(array<T, N>&& a) noexcept {
  static_assert(I < N, "array index out of bounds");
  return std::move(a.m_elems[I]);
}
template <std::size_t I, class T, std::size_t N>
constexpr const T&& get(const array<T, N>&& a) noexcept {
  static_assert(I < N, "array index out of bounds");
  return std::move(a.m_elems[I]);
}

} // namespace dss

template <typename T, std::size_t N>
struct std::tuple_size<dss::array<T, N>>
    : std::integral_constant<std::size_t, N> {};
template <std::size_t I, typename T, std::size_t N>
struct std::tuple_element<I, dss::array<T, N>> {
  static_assert(I < N, "array index out of bounds");
  using type = T;
};

#endif // ARRAY_H
