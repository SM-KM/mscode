#ifndef QUEUE_H
#define QUEUE_H

#include <iterator>
#include <type_traits>

#include "../deque/deque.h"
#include "../utils/utils.h"

namespace dss {

template <class T, class Container = deque<T>>
class queue {
  using container_type = Container;
  using value_type = Container::value_type;
  using size_type = Container::size_type;
  using reference = Container::reference;
  using const_reference = Container::const_reference;

  queue() : queue(Container()) {}
  explicit queue(const Container& cont);
  explicit queue(Container&& cont);
  queue(const queue& other);
  queue(queue&& other);
  template <std::input_iterator InputIt>
  queue(InputIt first, InputIt last);
  template <class Alloc>
  explicit queue(const Alloc& alloc);
  template <class Alloc>
  queue(const Container& cont, const Alloc& alloc);
  template <class Alloc>
  queue(Container&& cont, const Alloc& alloc);
  template <class Alloc>
  queue(const queue& other, const Alloc& alloc);
  template <class Alloc>
  queue(queue&& other, const Alloc& alloc);
  template <class InputIt, class Alloc>
  queue(InputIt first, InputIt last, const Alloc& alloc);

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L
  template <container_compatible_range<T> R>
  queue(std::from_range_t, R&& rg);
  template <container_compatible_range<T> R, class Alloc>
  queue(std::from_range_t, R&& rg, const Alloc& alloc);

  template <container_compatible_range<value_type> R>
  void push_range(R&& rg);
#endif

  ~queue();

  queue& operator=(const queue& other);
  queue& operator=(queue&& other);

  reference front();
  const_reference front() const;
  [[nodiscard]] bool empty() const;
  size_type size() const;

  void push(const value_type& value);
  void push(value_type&& value);

  template <class... Args>
  void emplace(Args&&... args);
  template <class... Args>
  decltype(auto) emplace(Args&&... args);
  void pop();

  void swap(queue& other) noexcept(std::is_nothrow_swappable_v<Container>);

 protected:
  Container m_c;
};

template <class T, std::three_way_comparable Container>
std::compare_three_way_result_t<Container> operator<=>(
    const queue<T, Container>& lhs, const queue<T, Container>& rhs);

template <std::ranges::input_range R>
queue(std::from_range_t, R&&) -> queue<std::ranges::range_value_t<R>>;
template <std::ranges::input_range R, class Allocator>
queue(std::from_range_t, R&&, Allocator)
    -> queue<std::ranges::range_value_t<R>,
             deque<std::ranges::range_value_t<R>, Allocator>>;

} // namespace dss

#endif // QUEUE_H
