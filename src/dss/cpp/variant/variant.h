#ifndef VARIANT_H
#define VARIANT_H

#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace dss {

template <typename... types>
class variant;

inline constexpr std::size_t variant_npos = static_cast<std::size_t>(-1);

// variant alternative, cause variant size only works for variant
template <typename V>
struct variant_size;
template <typename... Ts>
struct variant_size<variant<Ts...>>
    : std::integral_constant<std::size_t, sizeof...(Ts)> {};
template <typename V>
inline constexpr std::size_t variant_size_v = variant_size<V>::value;

// variant_alternative
template <std::size_t I, typename V>
struct variant_alternative;
template <std::size_t I, typename... Ts>
struct variant_alternative<I, variant<Ts...>> {
  static_assert(I < sizeof...(Ts), "variant index out of bounds");
  using type = std::tuple_element<I, std::tuple<Ts...>>;
};

template <std::size_t I, typename V>
struct variant_alternative<I, const V> {
  using type = std::add_const_t<typename variant_alternative<I, V>::type>;
};
template <std::size_t I, typename V>
using variant_alternative_t = typename variant_alternative<I, V>::type;

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

  constexpr ~variant();

  template <class T>
  constexpr variant& operator=(T&& t) noexcept;

  constexpr std::size_t index() const noexcept;
  constexpr bool valueless_by_exception() const noexcept;

  // emplace
  template <class T, class... Args>
  constexpr T& emplace(Args&&... args);

  template <class T, class U, class... Args>
  constexpr T& emplace(std::initializer_list<U> il, Args&&... args);

  template <std::size_t I, class... Args>
  constexpr std::variant_alternative_t<I, variant>& emplace(Args&&... args);

  template <std::size_t I, class U, class... Args>
  constexpr std::variant_alternative_t<I, variant>& emplace(
      std::initializer_list<U> il, Args&&... args);

  // swap
  constexpr void swap(variant& rhs) noexcept;
};

// Organize so that it can work with my variant
template <class... Types>
constexpr std::common_comparison_category_t<
    std::compare_three_way_result_t<Types>...>
operator<=>(const std::variant<Types...>& v, const std::variant<Types...>& w);

template <class R, class Visitor, class... Variants>
constexpr R visit(Visitor&& vis, Variants&&... vars);

template <class T, class... Types>
constexpr bool holds_alternative(const std::variant<Types...>& v) noexcept;

// std::get(std::variant)
template <std::size_t I, class... Types>
constexpr std::variant_alternative_t<I, std::variant<Types...>>& get(
    std::variant<Types...>& v);
template <std::size_t I, class... Types>
constexpr std::variant_alternative_t<I, std::variant<Types...>>&& get(
    std::variant<Types...>&& v);
template <std::size_t I, class... Types>
constexpr const std::variant_alternative_t<I, std::variant<Types...>>& get(
    const std::variant<Types...>& v);
template <std::size_t I, class... Types>
constexpr const std::variant_alternative_t<I, std::variant<Types...>>&& get(
    const std::variant<Types...>&& v);

template <class T, class... Types>
constexpr T& get(std::variant<Types...>& v);
template <class T, class... Types>
constexpr T&& get(std::variant<Types...>&& v);
template <class T, class... Types>
constexpr const T& get(const std::variant<Types...>& v);
template <class T, class... Types>
constexpr const T&& get(const std::variant<Types...>&& v);

template <std::size_t I, class... Types>
constexpr std::add_pointer_t<
    std::variant_alternative_t<I, std::variant<Types...>>>
get_if(std::variant<Types...>* pv) noexcept;
template <std::size_t I, class... Types>
constexpr std::add_pointer_t<
    const std::variant_alternative_t<I, std::variant<Types...>>>
get_if(const std::variant<Types...>* pv) noexcept;

template <class T, class... Types>
constexpr std::add_pointer_t<T> get_if(std::variant<Types...>* pv) noexcept;
template <class T, class... Types>
constexpr std::add_pointer_t<const T> get_if(
    const std::variant<Types...>* pv) noexcept;

} // namespace dss

#endif // VARIANT_H
