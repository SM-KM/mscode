#ifndef STACK_H
#define STACK_H

#include <iterator>
#include <type_traits>

#include "../deque/deque.h"
#include "../utils/utils.h"

namespace dss {
template <typename T, typename Container = dss::deque<T> >
class stack {
  Container c;

  using container_type = Container;
  using value_type = Container::value_type;
  using size_type = Container::size_type;
  using reference = Container::reference;
  using const_reference = Container::const_reference;

  stack() : stack(Container()) {}
  explicit stack(const Container& cont);
  explicit stack(Container&& cont);
  stack(const stack& other);
  stack(stack&& other);

  template <std::input_iterator InputIt>
  stack(InputIt first, InputIt last);
  template <class Alloc>
  explicit stack(const Alloc& alloc);
  template <class Alloc>
  stack(const Container& cont, const Alloc& alloc);
  template <class Alloc>
  stack(Container&& cont, const Alloc& alloc);
  template <class Alloc>
  stack(const stack& other, const Alloc& alloc);
  template <class Alloc>
  stack(stack&& other, const Alloc& alloc);

  template <std::input_iterator InputIt, class Alloc>
  stack(InputIt first, InputIt last, const Alloc& alloc);

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<T> R>
  stack(std::from_range_t, R&& rg);

  template <container_compatible_range<T> R, class Alloc>
  stack(std::from_range_t, R&& rg, const Alloc& alloc);

  template <container_compatible_range<value_type> R>
  void push_range(R&& rg);
#endif

  ~stack();

  stack& operator=(const stack& other);
  stack& operator=(stack&& other);

  reference top();
  const_reference top() const;

  [[nodiscard]] bool empty() const;
  size_type size() const;

  void push(const value_type& value);
  void push(value_type&& value);

  template <class... Args>
  decltype(auto) emplace(Args&&... args);

  void pop();
  void swap(stack& other) noexcept(std::is_nothrow_swappable_v<Container>);
};

template <class T, std::three_way_comparable Container>
std::compare_three_way_result_t<Container> operator<=>(
    const stack<T, Container>& lhs, const stack<T, Container>& rhs);

template <std::input_iterator InputIt>
stack(InputIt, InputIt)
    -> stack<typename std::iterator_traits<InputIt>::value_type>;

} // namespace dss

#endif // STACK_H
