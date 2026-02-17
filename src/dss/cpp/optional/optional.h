#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <utility>

namespace dss
{

struct nullopt_t
{
  constexpr explicit nullopt_t(int) {}
};

template <typename T>
class optional
{
public:
  using value_type = T;

  constexpr optional() noexcept;
  constexpr optional(dss::nullopt_t) noexcept;
  constexpr optional(const optional& other);
  constexpr optional(optional&& other) noexcept;

  template <class U>
  constexpr optional(const optional<U>& other);

  template <class U>
  constexpr optional(optional<U>&& other);

  template <class... Args>
  constexpr explicit optional(std::in_place_t, Args&&...args);

  template <class U, class... Args>
  constexpr explicit optional(std::in_place_t, std::initializer_list<U> ilist,
                              Args&&...args);

  template <class U = T>
  constexpr optional(U&& value);

  constexpr ~optional();

  // operators
  constexpr const T *operator->() const noexcept;
  constexpr T *operator->() noexcept;
  constexpr const T& operator*() const& noexcept;
  constexpr T& operator*() & noexcept;
  constexpr const T&& operator*() const&& noexcept;
  constexpr T&& operator*() && noexcept;

  constexpr T& value() &;
  constexpr const T& value() const&;
  constexpr T&& value() &&;
  constexpr const T&& value() const&&;

  // value or is the core of getting safely values from optional
  template <class U>
  constexpr T value_or(U&& default_value) const&;
  template <class U>
  constexpr T value_or(U&& default_value) &&;

  constexpr void reset() noexcept;

  // emplaceing the object on op
  template <class... Args>
  constexpr T& emplace(Args&&...args);
  template <class U, class... Args>
  constexpr T& emplace(std::initializer_list<U> ilist, Args&&...args);
};

// make optional
template <class T>
constexpr dss::optional<std::decay_t<T>> make_optional(T&& value);
template <class T, class... Args>
constexpr dss::optional<T> make_optional(Args&&...args);
template <class T, class U, class... Args>
constexpr dss::optional<T> make_optional(std::initializer_list<U> il,
                                         Args&&...args);

} // namespace dss

#endif // OPTIONAL_H
