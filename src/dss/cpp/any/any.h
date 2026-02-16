#ifndef ANY_H
#define ANY_H

#include <initializer_list>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace dss
{

// Any is not a templated class is an implementation from C++17,
// that allows to hold any time
class any
{
  constexpr any() noexcept;
  any(const any& other);
  any(const any&& other) noexcept;
  ~any();

  template <typename ValueType>
  any(ValueType&& value);

  template <typename ValueType, class... Args>
  explicit any(std::in_place_type_t<ValueType>, Args&&...args);

  template <typename ValueType, typename U, typename... Args>
  explicit any(std::in_place_type_t<ValueType>, std::initializer_list<U> il,
               Args&&...args);

  any& operator=(const any& rhs);
  any& operator=(any&& rhs) noexcept;
  template <typename ValueType>
  any& operator=(ValueType&& rhs);

  // Most operations on std::any decays the valuetype
  template <typename ValueType, typename... Args>
  std::decay_t<ValueType>& emplace(Args&&...args);
  template <typename ValueType, typename U, typename... Args>
  std::decay_t<ValueType>& emplace(std::initializer_list<U> il, Args&&...args);

  // Destroy the contained object
  void reset() noexcept;

  void swap(any& other) noexcept;
  bool has_value() const noexcept;

  // std::type_info describes the information of the type stored if any
  const std::type_info& type() const noexcept;
};

// any_cast
template <typename T>
T any_cast(const any& operand);

template <typename T>
T any_cast(any& operand);

template <typename T>
T any_cast(any&& operand);

template <typename T>
const T *any_cast(const any *operand) noexcept;

template <typename T>
T *any_cast(any *operand) noexcept;

// make_any

} // namespace dss

#endif // ANY_H
