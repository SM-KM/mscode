#ifndef VARIANT_H
#define VARIANT_H

#include <initializer_list>
#include <utility>
namespace dss {

template <typename... types>
class variant {
  constexpr variant() noexcept;
  constexpr variant(const variant& other);
  constexpr variant(variant&& other) noexcept;

  template <typename T>
  constexpr variant(T&& t) noexcept;

  template <typename T, typename... Args>
  constexpr explicit variant(std::in_place_type_t<T>, Args&&... args);
  template <typename T, typename U, typename... Args>
  constexpr explicit variant(std::in_place_type_t<T>,
                             std::initializer_list<U> il, Args&&... args);

  template <std::size_t I, class... Args>
  constexpr explicit variant(std::in_place_index_t<I>, Args&&... args);

  template <std::size_t I, class U, class... Args>
  constexpr explicit variant(std::in_place_index_t<I>,
                             std::initializer_list<U> il, Args&&... args);
};

} // namespace dss

#endif // VARIANT_H
