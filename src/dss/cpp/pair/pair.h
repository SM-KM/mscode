#ifndef PAIR_H
#define PAIR_H

#include <type_traits>
#include <utility>
namespace dss {

template <typename U1, typename U2>
concept pairlike =
    std::is_constructible_v<U1, U1> && std::is_constructible_v<U2, U2>;

template <typename T1, typename T2>
struct pair {
  using first_type = T1;
  using second_type = T2;

  // is conditionally explicit
  constexpr explicit pair();
  constexpr explicit pair(const T1& x, const T2& y);

  template <class U1 = T1, class U2 = T2>
  constexpr pair(U1&& x, U2&& y);

  template <class U1, class U2>
  constexpr pair(pair<U1, U2>& p);

  template <class U1, class U2>
  constexpr pair(const pair<U1, U2>& p);

  template <class U1, class U2>
  constexpr pair(pair<U1, U2>&& p);
  template <class U1, class U2>
  constexpr pair(const pair<U1, U2>&& p);

  template <pairlike<T1> P>
  constexpr pair(P&& u);

  template <class... Args1, class... Args2>
  constexpr pair(std::piecewise_construct_t, std::tuple<Args1...> first_args,
                 std::tuple<Args2...> second_args);

  pair(const pair& p) = default;
  pair(pair&& p) = default;
};
} // namespace dss
#endif // PAIR_H
