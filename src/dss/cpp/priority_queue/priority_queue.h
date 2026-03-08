#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "../utils/utils.h"
#include "../vector/vector.h"

namespace dss {

template <class T, class Container = vector<T>,
          class Compare = std::less<typename Container::value_type>>
class priority_queue {
  using container_type = Container;
  using value_type = Container::value_type;
  using size_type = Container::size_type;
  using reference = Container::reference;
  using const_reference = Container::const_reference;

  priority_queue() : priority_queue(Compare(), Container()) {}
  explicit priority_queue(const Compare& compare)
      : priority_queue(compare, Container()) {}
  priority_queue(const Compare& compare, const Container& cont);
  priority_queue(const Compare& compare, Container&& cont);
  priority_queue(const priority_queue& other);
  priority_queue(priority_queue&& other);
  template <class InputIt>
  priority_queue(InputIt first, InputIt last,
                 const Compare& compare = Compare());
  template <class InputIt>
  priority_queue(InputIt first, InputIt last,
                 const Compare& compare = Compare(),
                 const Container& cont = Container());

  template <class InputIt>
  priority_queue(InputIt first, InputIt last, const Compare& compare,
                 Container&& cont);
  template <class Alloc>
  explicit priority_queue(const Alloc& alloc);
  template <class Alloc>
  priority_queue(const Compare& compare, const Alloc& alloc);
  template <class Alloc>
  priority_queue(const Compare& compare, const Container& cont,
                 const Alloc& alloc);
  template <class Alloc>
  priority_queue(const Compare& compare, Container&& cont, const Alloc& alloc);
  template <class Alloc>
  priority_queue(const priority_queue& other, const Alloc& alloc);
  template <class Alloc>
  priority_queue(priority_queue&& other, const Alloc& alloc);
  template <class InputIt, class Alloc>
  priority_queue(InputIt first, InputIt last, const Alloc& alloc);
  template <class InputIt, class Alloc>
  priority_queue(InputIt first, InputIt last, const Compare& compare,
                 const Alloc& alloc);
  template <class InputIt, class Alloc>
  priority_queue(InputIt first, InputIt last, const Compare& compare,
                 const Container& cont, const Alloc& alloc);
  template <class InputIt, class Alloc>
  priority_queue(InputIt first, InputIt last, const Compare& compare,
                 Container&& cont, const Alloc& alloc);

#if defined(__cpp_lib_containers_ranges) && \
    __cpp_lib_containers_ranges >= 202202L

  template <container_compatible_range<T> R>
  priority_queue(std::from_range_t, R&& rg, const Compare& compare = Compare());
  template <container_compatible_range<T> R, class Alloc>
  priority_queue(std::from_range_t, R&& rg, const Compare& compare,
                 const Alloc& alloc);
  template <container_compatible_range<T> R, class Alloc>
  priority_queue(std::from_range_t, R&& rg, const Alloc& alloc);

  template <container_compatible_range<value_type> R>
  void push_range(R&& rg);
#endif

  ~priority_queue();

  priority_queue& operator=(const priority_queue& other);
  priority_queue& operator=(priority_queue&& other);
  const_reference top() const;
  [[nodiscard]] bool empty() const;
  size_type size() const;

  void push(const value_type& value);
  void push(value_type&& value);

  template <class... Args>
  void emplace(Args&&... args);
  void pop();
  void swap(priority_queue& other) noexcept(
      std::is_nothrow_swappable_v<Container>);

 protected:
  Container m_c;
  Compare m_comp;
};

template <std::ranges::input_range R,
          class Comp = std::less<std::ranges::range_value_t<R>>>
priority_queue(std::from_range_t, R&&, Comp = Comp())
    -> priority_queue<std::ranges::range_value_t<R>,
                      std::vector<std::ranges::range_value_t<R>>, Comp>;
template <std::ranges::input_range R, class Comp, class Alloc>
priority_queue(std::from_range_t, R&&, Comp, Alloc)
    -> priority_queue<std::ranges::range_value_t<R>,
                      std::vector<std::ranges::range_value_t<R>, Alloc>, Comp>;
template <std::ranges::input_range R, class Alloc>
priority_queue(std::from_range_t, R&&, Alloc)
    -> priority_queue<std::ranges::range_value_t<R>,
                      std::vector<std::ranges::range_value_t<R>, Alloc>>;
} // namespace dss

#endif // PRIORITY_QUEUE_H
