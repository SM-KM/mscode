#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include <concepts>
#include <iostream>
#include <memory>

namespace dss {

template <typename T> class SingleLinkedList_Node {};

template <typename T, class Allocator = std::allocator<T>, bool Debug = false>
class SingleLinkedList {
private:
  bool debug_ = Debug;
  SingleLinkedList_Node<T> root_;
  SingleLinkedList_Node<T> tail_;

public:
  // Custom iterator
  struct iterator {
    T m_ptr;

    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;

    iterator(pointer p) : m_ptr(p) {}
    reference operator*() { return *m_ptr; }
    pointer operator->() { return m_ptr; }

    iterator& operator++() {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    iterator operator++(T) {
      ++m_ptr;
      return *this;
    }

    friend bool operator==(const iterator& a, const iterator& b) {
      return a.m_ptr == b.m_ptr;
    }
    friend bool operator!=(const iterator& a, const iterator& b) {
      return a.m_ptr != b.m_ptr;
    }
  };

  using value_type = T;
  using allocator_type = Allocator;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;

  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = iterator;
  using const_iterator = const iterator;
  using size_type = size_t;

  // Methods
  iterator begin() { return iterator(&root_); }
  iterator end() { return iterator(nullptr); }

  // Cool improvement C++20 enforce that the param passed to remove_if is
  // callable using std::predicate<T> that would throw an error before compile
  // time, so basically before it tries to call predicate and the thing cannot
  // be called
  template <typename Predicate> size_type remove_if(Predicate pred);
};

} // namespace dss

#endif // SINGLELINKEDLIST_H
