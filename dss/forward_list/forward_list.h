#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <concepts>
#include <forward_list>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <utility>

namespace dss {
template <typename T> struct forward_list_node {
  T m_data;
  forward_list_node* m_next;

  forward_list_node() {
    m_data = T();
    m_next = nullptr;
  };

  forward_list_node(T data) {
    m_data = data;
    m_next = nullptr;
  };

  ~forward_list_node();

  forward_list_node(const forward_list_node& sln) {
    m_data = sln.m_data;
    m_next = sln.m_next;
  };

  forward_list_node& operator=(const forward_list_node& sln) {
    std::swap(m_data, sln.m_data);
    std::swap(m_next, sln.m_next);
    return *this;
  };

  forward_list_node(forward_list_node&& sln) {
    m_data = sln.m_data;
    m_next = sln.m_next;
    sln.m_data = T();
    sln.m_next = nullptr;
  };

  forward_list_node& operator=(forward_list_node&& sln) {
    if (this != sln) {
      delete m_data;
      delete m_next;

      m_data = std::move(sln.m_data);
      m_next = std::move(sln.m_next);
      sln.m_data = T();
      sln.m_next = nullptr;
    }

    return *this;
  };
};

template <typename T, class Allocator = std::allocator<T>, bool Debug = false>
class forward_list {
private:
  bool debug_ = Debug;
  forward_list_node<T> root_;
  forward_list_node<T> tail_;

public:
  class iterator {
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

  // ctors, dtor
  forward_list() : forward_list(Allocator()) {};
  explicit forward_list(const Allocator&);
  explicit forward_list(size_type n, const Allocator& = Allocator());
  forward_list(std::initializer_list<T>);
  forward_list(size_type n, const T& value, const Allocator& = Allocator());
  ~forward_list();

  // to create SingleLinkedList from other stl containers
  template <typename InputIt>
  forward_list(InputIt begin, InputIt end, const Allocator& = Allocator());

  // copy
  forward_list(const forward_list& other);
  forward_list& operator=(const forward_list& other);

  // move
  forward_list(forward_list&& other);
  forward_list& operator=(forward_list&& other);

  // iterators
  iterator begin() noexcept { return iterator(&root_); };
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;
  iterator end() noexcept;
  iterator rbegin() noexcept;
  iterator rend() noexcept;

  size_t size();

  // assign elements from other stl container, using iterators
  template <typename InputIt> void assign(InputIt begin, InputIt end);
  void assign(size_type n, const T& t);
  void assign(std::initializer_list<T>);

  [[nodiscard]] bool empty() const noexcept;
  size_type max_size() const noexcept;

  // element access
  reference front();
  const_reference front() const;

  // modifiers
  template <typename... Args>
  reference emplace_front(Args&&... args); // think well how this works

  void push_front(const T& x);
  void push_front(T&& x);
  void pop_front();
  void clear() noexcept;

  template <typename... Args>
  iterator emplace_after(const_iterator position, Args&&... args);
  iterator insert_after(const_iterator position, const T& x);
  iterator insert_after(const_iterator position, T&& x);
  iterator insert_after(const_iterator position, size_type n, const T& x);

  iterator erase_after(const_iterator position);
  iterator erase_after(const_iterator begin, const_iterator end);

  // noexpect compares if the allocator passed to the SingleLinkedList to swap
  // are equal
  void swap(forward_list& sl) noexcept(
      std::allocator_traits<Allocator>::is_always_equal::value);

  void resize(size_type s);
  void resize(size_type s,
              const value_type& t); // if the rezised SingleLinkedList is
                                    // smaller than s, elements with value_type
                                    // are added untill the new size is matched

  void splice_after(const_iterator position, forward_list& sl);
  void splice_after(const_iterator position, forward_list&& sl);
  void splice_after(const_iterator position, forward_list& sl,
                    const_iterator i);
  void splice_after(const_iterator position, forward_list&& sl,
                    const_iterator i);
  void splice_after(const_iterator position, forward_list& sl,
                    const_iterator begin, const_iterator end);
  void splice_after(const_iterator position, forward_list&& sl,
                    const_iterator begin, const_iterator end);

  // Cool improvement C++20 enforce that the param passed to remove_if is
  // callable using std::predicate<T> that would throw an error before compile
  // time, so basically before it tries to call predicate and the thing cannot
  // be called
  size_type remove(const T& t);
  template <typename Predicate> size_type remove_if(Predicate pred);

  size_type unique(); // deletes all duplicated of elements, but not permantly
                      // the memory still needs to be handled

  template <typename BinaryPredicate>
  size_type unique(BinaryPredicate binary_pred);

  void merge(forward_list& sl);
  void merge(forward_list&& sl);
  template <typename Compare> void merge(forward_list& sl, Compare comp);
  template <typename Compare> void merge(forward_list&& sl, Compare comp);

  void sort();
  template <typename Compare> void sort(Compare comp);
  void reverse() noexcept;
};

} // namespace dss
#endif // SINGLELINKEDLIST_H
