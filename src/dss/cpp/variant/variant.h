#ifndef VARIANT_H
#define VARIANT_H

#include <algorithm>
#include <initializer_list>
#include <memory>
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
  template <std::size_t I>
  using alt_t = std::tuple_element_t<I, std::tuple<types...>>;
  using index_seq = std::index_sequence_for<types...>;
  friend struct detail::variant_access;

 public:
  constexpr variant() noexcept(
      std::is_nothrow_default_constructible_v<alt_t<0>>) {
    construct<0>();
  };

  // move and copy constructors -> if other is valueless so are we, else
  // construct the alternatives other is holding
  constexpr variant(const variant& other) {
    detail::dispatch(
        other.m_index,
        [&](auto I) {
          constexpr std::size_t i = decltype(I)::value;
          this->template construct<i>(
              detail::variant_access::get_unchecked<i>(other));
        },
        index_seq{});
  };

  constexpr variant(variant&& other) noexcept(
      (std::is_nothrow_move_constructible_v<types> && ...)) {
    detail::dispatch(
        other.m_index,
        [&](auto I) {
          constexpr std::size_t i = decltype(I)::value;
          this->template construct<i>(
              detail::variant_access::get_unchecked<i>(std::move(other)));
        },
        index_seq{});
  };

  // T picks the alternative by overload resolution (constrained so it does)
  // not steal the copy / move constructors or the in_place ones
  template <typename T>
    requires(!std::is_same_v<std::remove_cvref_t<T>, variant> &&
             !detail::is_in_place_tag<std::remove_cvref_t<T>>::value &&
             detail::has_match<T, types...>)
  constexpr variant(T&& t) noexcept(
      std::is_nothrow_constructible_v<
          alt_t<detail::selected_index_v<T, types...>>, T>) {
    construct<detail::selected_index_v<T, types...>>(std::forward<T>(t));
  };

  template <typename T, typename... Args>
  constexpr explicit variant(std::in_place_type_t<T>, Args&&... args) {
    static_assert(detail::count_of_v<T, types...> == 1,
                  "T must appear exactly once in the variant");
    construct<detail::index_of_v<T, types...>>(std::forward<Args>(args)...);
  }
  template <typename T, typename U, typename... Args>
  constexpr explicit variant(std::in_place_type_t<T>,
                             std::initializer_list<U> il, Args&&... args) {
    static_assert(detail::count_of_v<T, types...> == 1,
                  "T must appear exactly once in the variant");
    construct<detail::index_of_v<T, types...>>(il, std::forward<Args>(args)...);
  }
  template <std::size_t I, class... Args>
  constexpr explicit variant(std::in_place_index_t<I>, Args&&... args) {
    construct<I>(std::forward<Args>(args)...);
  }

  template <std::size_t I, class U, class... Args>
  constexpr explicit variant(std::in_place_index_t<I>,
                             std::initializer_list<U> il, Args&&... args) {
    construct<I>(std::forward<Args>(args)...);
  }
  constexpr ~variant() { reset(); };

  template <typename T>
    requires(!std::is_same_v<std::remove_cvref_t<T>, variant> &&
             detail::has_match<T, types...>)
  constexpr variant& operator=(T&& t) noexcept(
      std::is_nothrow_assignable_v<alt_t<detail::selected_index_v<T, types...>>,
                                   T> &&
      std::is_nothrow_constructible_v<
          alt_t<detail::selected_index_v<T, types...>>, T>) {
    constexpr std::size_t j = detail::selected_index_v<T, types...>;
    using Tj = alt_t<j>;

    if (m_index == j) {
      detail::variant_access::get_unchecked<j>(*this) = std::forward<T>(t);
    } else if constexpr (std::is_nothrow_constructible_v<Tj, T> ||
                         !std::is_nothrow_move_constructible_v<Tj>) {
      emplace<j>(std::forward<T>(t));
    } else {
      emplace<j>(Tj(std::forward<T>(t)));
    }
    return *this;
  }

  constexpr std::size_t index() const noexcept { return m_index; };
  constexpr bool valueless_by_exception() const noexcept {
    return m_index == variant_npos;
  }

  // emplace destroys the current value first, so if the construction throws
  // the variant is left valueless
  template <class T, class... Args>
  constexpr T& emplace(Args&&... args) {
    static_assert(detail::count_of_v<T, types...>,
                  "T must appear in exactly once in the variant");
    return emplace<detail::index_of_v<T, types...>>(
        std::forward<Args>(args)...);
  }
  template <class T, class U, class... Args>
  constexpr T& emplace(std::initializer_list<U> il, Args&&... args) {
    static_assert(detail::count_of_v<T, types...> == 1,
                  "T must appear exactly once in the variant");
    return emplace<detail::index_of_v<T, types...>>(
        il, std::forward<Args>(args)...);
  }
  template <std::size_t I, class... Args>
  constexpr variant_alternative_t<I, variant>& emplace(Args&&... args) {
    reset();
    construct<I>(std::forward<Args>(args)...);
    return detail::variant_access::get_unchecked<I>(*this);
  }

  template <std::size_t I, class U, class... Args>
  constexpr variant_alternative_t<I, variant>& emplace(
      std::initializer_list<U> il, Args&&... args) {
    reset();
    construct<I>(il, std::forward<Args>(args)...);
    return detail::variant_access::get_unchecked<I>(*this);
  }

  // swap
  constexpr void swap(variant& rhs) noexcept(
      ((std::is_nothrow_move_constructible_v<types> &&
        std::is_nothrow_swappable_v<types>) &&
       ...)) {
    if (m_index == variant_npos && rhs.m_index == variant_npos) return;
    if (m_index == rhs.m_index) {
      detail::dispatch(
          m_index,
          [&](auto I) {
            constexpr std::size_t i = decltype(I)::value;
            using std::swap;
            swap(detail::variant_access::get_unchecked<i>(*this),
                 detail::variant_access::get_unchecked<i>(rhs));
          },
          index_seq{});
    } else {
      // all the different alternatives go through alternatives
      variant tmp{std::move(rhs)};
      rhs = std::move(*this);
      *this = std::move(tmp);
    }
  }

  // construct alternative I in the storage, we must be valueless before. the
  // index is set after the construction, so a throw leaves us valueless
  template <std::size_t I, typename... Args>
  constexpr void construct(Args&&... args) {
    std::construct_at(reinterpret_cast<alt_t<I>*>(m_storage),
                      std::forward<Args>(args)...);
  }

 private:
  // destroy whenever we hold and become valueless
  constexpr void reset() noexcept {
    if (m_index == variant_npos) return;
    detail::dispatch(
        m_index,
        [&](auto I) {
          constexpr std::size_t i = decltype(I)::value;
          std::destroy_at(
              std::addressof(detail::variant_access::get_unchecked<i>(*this)));
        },
        index_seq{});
  }

  alignas(types...) unsigned char m_storage[std::max({sizeof(types)...})];
  std::size_t m_index{variant_npos};
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
