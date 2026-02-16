#ifndef ANY_H
#define ANY_H

// Any is not a templated class is an implementation from C++17,
// that allows to hold any time
#include <initializer_list>
#include <type_traits>
#include <utility>
class any
{
  constexpr any() noexcept;
  any(const any& other);
  any(const any&& other) noexcept;

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

  ~any();
};

#endif // ANY_H
