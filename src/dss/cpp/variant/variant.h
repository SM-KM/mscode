#ifndef VARIANT_H
#define VARIANT_H

#include <algorithm>
#include <initializer_list>
#include <new>
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

namespace detail {
// allow me to check how many times T appears in Ts and where with the
// index_of_v
template <typename T, typename... Ts>
inline constexpr std::size_t count_of_v =
    (static_cast<std::size_t>(std::is_same_v<T, Ts>) + ... + 0);

template <typename T, typename... Ts>
inline constexpr std::size_t index_of_v = [] {
  constexpr bool match[] = {std::is_same_v<T, Ts>...};
  // WARN: why would do pre-increment be a difference over post increment
  for (std::size_t i = 0; i < sizeof...(Ts); ++i)
    if (match[i]) return i;
  return variant_npos;
}();

// converting constructor / assignment: which alternative does T select?
// one imaginary F(ti) per alternative overload resolution picks the winner
// narrowing conversions are rejected like in the standard
template <typename Ti>
struct arr {
  Ti x[1];
};
template <typename Ti, typename U>
concept non_narrowing = requires { arr<Ti>{{std::declval<U>()}}; };
template <std::size_t I, typename Ti, typename U>
struct overload_leaf {
  static std::integral_constant<std::size_t, I> f(Ti)
    requires non_narrowing<Ti, U>;
};
template <typename U, typename Seq, typename... Ts>
struct overload_set;
template <typename U, std::size_t... Is, typename... Ts>
struct overload_set<U, std::index_sequence<Is...>, Ts...>
    : overload_leaf<Is, Ts, U>... {
  using overload_leaf<Is, Ts, U>::f...;
};

template <typename T, typename... Ts>
concept has_match = requires {
  overload_set<T, std::index_sequence_for<Ts...>, Ts...>::f(std::declval<T>());
};
template <typename T, typename... Ts>
inline constexpr std::size_t selected_index_v =
    decltype(overload_set<T, std::index_sequence_for<Ts...>, Ts...>::f(
        std::declval<T>()))::value;

template <typename T>
struct is_in_place_tag : std::false_type {};
template <typename T>
struct is_in_place_tag<std::in_place_type_t<T>> : std::true_type {};
template <std::size_t I>
struct is_in_place_tag<std::in_place_index_t<I>> : std::true_type {};

// runtime index -> compile time index (f gets a std::integral_constant)
template <typename F, std::size_t... Is>
constexpr void dispatch(std::size_t idx, F&& f, std::index_sequence<Is...>) {
  (void)((idx == Is ? (f(std::integral_constant<std::size_t, Is>{}), true)
                    : false) ||
         ...);

  // the only one that looks inside the storage, no index check, keeps
  // constness and value category of the variant passed
}

struct variant_access {
  template <std::size_t I, typename V>
  static constexpr auto&& get_unchecked(V&& v) noexcept {
    using T = variant_alternative<I, std::remove_cvref_t<V>>;
    using Q = std::conditional_t<std::is_const_v<std::remove_reference_t<V>>,
                                 const T, T>;

    Q* p = std::launder(reinterpret_cast<Q*>(v.m_storage));
    if constexpr (std::is_lvalue_reference_v<V>) {
      return *p;
    } else {
      return std::move(*p);
    }
  };
};

}; // namespace detail

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
