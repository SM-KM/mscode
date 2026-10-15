#ifndef PAIR_H
#define PAIR_H

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
namespace dss {

template <typename T1, typename T2>
struct pair;

namespace detail {

template <typename T>
struct is_dss_pair : std::false_type {};
template <typename T1, typename T2>
struct is_dss_pair<pair<T1, T2>> : std::true_type {};

/// The explicit of the default constructor depends on T x = {} compiling
template <typename T>
void implicit_default_test(const T&);
template <typename T, typename = void>
struct is_implicitly_default_constructible : std::false_type {};
template <typename T>
struct is_implicitly_default_constructible<
    T, std::void_t<decltype(implicit_default_test<T>({}))>> : std::true_type {};

/// get<I> on anything tuple like (std::tuple, std::array, std::pair)
template <std::size_t I, typename P>
constexpr decltype(auto) pair_get(P&& p) {
  using std::get;
  return get<I>(std::forward<P>(p));
}

template <typename P>
concept tuple_like_2 = requires {
  std::tuple_size<std::remove_cvref_t<P>>::value &&
      (std::tuple_size<std::remove_cvref_t<P>>::value == 2);
};

}; // namespace detail

template <typename P, typename T1, typename T2>
concept pairlike =
    !detail::is_dss_pair<std::remove_cvref<P>>::value &&
    detail::tuple_like_2<P> &&
    std::is_constructible_v<T1,
                            decltype(detail::pair_get<0>(std::declval<P>()))> &&
    std::is_constructible_v<T2,
                            decltype(detail::pair_get<1>(std::declval<P>()))>;

template <typename T1, typename T2>
struct pair {
  using first_type = T1;
  using second_type = T2;

  T1 first;
  T2 second;

  /// is conditionally explicit
  /// explicit only when T1 or T2 cannot be default constructable with T = {}
  constexpr explicit(!(detail::is_implicitly_default_constructible<T1>::value &&
                       detail::is_implicitly_default_constructible<T2>::value))
      pair()
    requires(std::is_default_constructible_v<T1> &&
             std::is_default_constructible_v<T2>)
      : first(), second(){};

  constexpr explicit(!(std::is_convertible_v<const T1&, T2> &&
                       std::is_convertible_v<const T2&, T1>))
      pair(const T1& x, const T2& y)
    requires(std::is_copy_constructible_v<T1> &&
             std::is_copy_constructible_v<T2>)
      : first{x}, second{y} {};

  template <typename U1 = T1, typename U2 = T2>
    requires(std::is_constructible_v<T1, U1> && std::is_constructible_v<T2, U2>)
  constexpr explicit(!(std::is_convertible_v<U1, T1> &&
                       std::is_convertible_v<U2, T2>)) pair(U1&& x, U2&& y)
      : first{std::forward<U1>(x)}, second{std::forward<U2>(y)} {};

  template <typename U1, typename U2>
    requires(std::is_constructible_v<T1, U1&> &&
             std::is_constructible_v<T2, U2&>)
  constexpr explicit(!(std::is_convertible_v<U1&, T1> &&
                       std::is_convertible_v<U2&, T2>)) pair(pair<U1, U2>& p)
      : first{p.first}, second{p.second} {};

  template <typename U1, typename U2>
  constexpr pair(const pair<U1, U2>& p);

  template <class U1, class U2>
  constexpr pair(pair<U1, U2>&& p);
  template <class U1, class U2>
  constexpr pair(const pair<U1, U2>&& p);

  // template <pairlike<T1> P>
  // constexpr pair(P&& u);

  template <class... Args1, class... Args2>
  constexpr pair(std::piecewise_construct_t, std::tuple<Args1...> first_args,
                 std::tuple<Args2...> second_args);

  pair(const pair& p) = default;
  pair(pair&& p) = default;

 private:
  /// the piecewise constructor need the indices of both tuples
  template <typename... Args1, typename... Args2, std::size_t... I1,
            std::size_t... I2>
  constexpr pair(std::piecewise_construct_t, std::tuple<Args1...>& a,
                 std::tuple<Args2...>& b, std::index_sequence<I1...>,
                 std::index_sequence<I2...>)
      : first{std::forward<Args1>(std::get<I1>(a))...},
        second{std::forward<Args2>(std::get<I2>(b))...} {};
};
} // namespace dss
#endif // PAIR_H
