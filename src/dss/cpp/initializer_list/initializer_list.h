#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H

#include <cstddef>
template <typename T>
class initializer_list {
  using value_type = T;
  using reference = const T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using iterator = const T*;
  using const_iterator = const T*;

  constexpr initializer_list() noexcept;
  constexpr size_type size() const noexcept;

  constexpr const T* begin() const noexcept;
  constexpr const T* end() const noexcept;
};

#endif // INITIALIZER_LIST_H
