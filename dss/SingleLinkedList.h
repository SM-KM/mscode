#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include <concepts>
#include <forward_list>
#include <initializer_list>
#include <iostream>
#include <memory>

namespace dss {

template <typename T> struct SingleLinkedList_Node {
  T data;
  SingleLinkedList_Node* next;

  SingleLinkedList_Node();
  SingleLinkedList_Node(T data);
  ~SingleLinkedList_Node();

  SingleLinkedList_Node(const SingleLinkedList_Node& sln);
  SingleLinkedList_Node& operator=(const SingleLinkedList_Node& sln);
  SingleLinkedList_Node(SingleLinkedList_Node&& sln);
  SingleLinkedList_Node& operator=(SingleLinkedList_Node&& sln);
};

template <typename T, class Allocator = std::allocator<T>, bool Debug = false>
class SingleLinkedList {
private:
  bool debug_ = Debug;
  SingleLinkedList_Node<T> root_;
  SingleLinkedList_Node<T> tail_;

public:
  // Custom iterator -- maybe implement outside
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

  // ctors, dtor
  SingleLinkedList() : SingleLinkedList(Allocator()) {};
  explicit SingleLinkedList(const Allocator&);
  explicit SingleLinkedList(size_type n, const Allocator& = Allocator());
  SingleLinkedList(std::initializer_list<T>);
  SingleLinkedList(size_type n, const T& value, const Allocator& = Allocator());
  ~SingleLinkedList();

  // to create SingleLinkedList from other stl containers
  template <typename InputIt>
  SingleLinkedList(InputIt begin, InputIt end, const Allocator& = Allocator());

  // copy
  SingleLinkedList(const SingleLinkedList& other);
  SingleLinkedList& operator=(const SingleLinkedList& other);

  // move
  SingleLinkedList(SingleLinkedList&& other);
  SingleLinkedList& operator=(SingleLinkedList&& other);

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
  void swap(SingleLinkedList& sl) noexcept(
      std::allocator_traits<Allocator>::is_always_equal::value);

  void resize(size_type s);
  void resize(size_type s,
              const value_type& t); // if the rezised SingleLinkedList is
                                    // smaller than s, elements with value_type
                                    // are added untill the new size is matched

  void splice_after(const_iterator position, SingleLinkedList& sl);
  void splice_after(const_iterator position, SingleLinkedList&& sl);
  void splice_after(const_iterator position, SingleLinkedList& sl,
                    const_iterator i);
  void splice_after(const_iterator position, SingleLinkedList&& sl,
                    const_iterator i);
  void splice_after(const_iterator position, SingleLinkedList& sl,
                    const_iterator begin, const_iterator end);
  void splice_after(const_iterator position, SingleLinkedList&& sl,
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

  void merge(SingleLinkedList& sl);
  void merge(SingleLinkedList&& sl);
  template <typename Compare> void merge(SingleLinkedList& sl, Compare comp);
  template <typename Compare> void merge(SingleLinkedList&& sl, Compare comp);

  void sort();
  template <typename Compare> void sort(Compare comp);
  void reverse() noexcept;
};

} // namespace dss
#endif // SINGLELINKEDLIST_H
