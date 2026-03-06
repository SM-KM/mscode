#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <memory>

namespace dss {

template <class T, class Deleter = std::default_delete<T> >
class unique_ptr {
  using pointer = T*;
  using element_type = T;
  using deleter_type = Deleter;

  constexpr unique_ptr() noexcept;
  constexpr unique_ptr(std::nullptr_t) noexcept;
  constexpr explicit unique_ptr(pointer p) noexcept;

  // is depenendant of the copy ctrs or move ctrs of the deleter
  unique_ptr(pointer p, const T& d1) noexcept;
  unique_ptr(pointer p, T&& d1) noexcept;

  unique_ptr(unique_ptr&& u) noexcept;
  unique_ptr(unique_ptr& u) = delete;

  template <class U, class E>
  unique_ptr(unique_ptr<U, E>&& u) noexcept;

  ~unique_ptr();

  constexpr unique_ptr& operator=(const unique_ptr&) = delete;
  constexpr unique_ptr& operator=(unique_ptr&& r) noexcept;
  template <class U, class E>
  constexpr unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept;
  constexpr unique_ptr& operator=(std::nullptr_t) noexcept;

  constexpr pointer release() noexcept;
  constexpr void reset(pointer ptr = pointer()) noexcept;
  void swap(unique_ptr& other) noexcept;
  pointer get() const noexcept { return m_ptr; };

  constexpr Deleter& get_deleter() noexcept;
  constexpr const Deleter& get_deleter() const noexcept;
  explicit operator bool() const noexcept;

  constexpr typename std::add_lvalue_reference<T>::type operator*() const
      noexcept(noexcept(*std::declval<pointer>()));
  constexpr pointer operator->() const noexcept;

 private:
  pointer m_ptr;
};

template <class T, class... Args>
constexpr unique_ptr<T> make_unique(Args&&... args);
template <class T>
constexpr unique_ptr<T> make_unique_for_overwrite();

template <class T, class D>
  requires std::three_way_comparable<typename unique_ptr<T, D>::pointer>
std::compare_three_way_result_t<typename unique_ptr<T, D>::pointer> operator<=>(
    const unique_ptr<T, D>& x, std::nullptr_t);

template <class CharT, class Traits, class Y, class D>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const std::unique_ptr<Y, D>& p);

} // namespace dss

#endif // UNIQUE_PTR_H
