#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <iterator>
#include <stdexcept>
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

  [[nodiscard]] constexpr bool empty() const noexcept;
  [[nodiscard]] constexpr size_type size() const noexcept;
  constexpr size_type max_size() const noexcept;
  void swap(array& other) noexcept(std::is_nothrow_swappable_v<T>);
  void fill(const T& value);

  // template <typename U, std::size_t S>
  // constexpr bool operator<=>(const std::array<T, N>& lhs,
  //                            const std::array<T, N>& rhs);
};

template <typename T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]);
template <typename T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&&a)[N]);
} // namespace dss

#endif // ARRAY_H
