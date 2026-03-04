#ifndef VECTOR_H
#define VECTOR_H

#include <concepts>
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dss {

template <typename T, typename R>
concept container_compatible_range =
    std::ranges::range<T> &&
    std::convertible_to<std::ranges::range_value_t<R>, T>;

template <typename T, typename Allocator = std::allocator<T>>
class vector {
  // considerations i want reference
  // access to iterators
  class Iterator {
    using iterator_category = std::contiguous_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = T&;
    using pointer = T*;

   public:
    pointer ptr;

    // Constructors
    constexpr Iterator() = default;
    constexpr explicit Iterator(pointer p) : ptr{p} {}

    // Operators
    constexpr T& operator*() { return *ptr; }
    constexpr bool operator==(const Iterator& other) const {
      return other.ptr == ptr;
    }

    constexpr Iterator& operator++() {
      ++ptr;
      return *this;
    }

    constexpr Iterator operator++(int) {
      auto prev = *this;
      ++(*this);
      return prev;
    }

    constexpr Iterator& operator--() {
      --ptr;
      return *this;
    }

    constexpr Iterator operator--(int) {
      auto prev = *this;
      ++(*this);
      return prev;
    }

    constexpr Iterator& operator+=(difference_type n) {
      ptr += n;
      return *this;
    }

    constexpr Iterator operator+(difference_type n) const {
      auto next = *this;
      next += n;
      return next;
    }

    constexpr Iterator& operator-=(difference_type n) {
      ptr -= n;
      return *this;
    }

    constexpr Iterator operator-(difference_type n) const {
      auto next = *this;
      next -= n;
      return next;
    }

    difference_type operator-(const Iterator& other) const {
      return ptr - other.ptr;
    }
    constexpr T& operator[](difference_type n) { return ptr[n]; }
    constexpr auto operator<=>(const Iterator& other) {
      return other.ptr <=> ptr;
    }
    constexpr T* operator->() const { return ptr->data(); }
  };

 public:
  using value_type = T;

  template <typename U>
  using type_identity_t = U;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;

  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using iterator = Iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using allocator_type = Allocator;

  constexpr allocator_type get_allocator() const noexcept {
    return m_allocator;
  };

  // the allocator constructor has to be no expect as well
  constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {};
  constexpr explicit vector(const Allocator& alloc) noexcept
      : m_allocator{alloc} {
    m_data = m_allocator.allocate(m_size);
  };

  constexpr explicit vector(size_type n, const Allocator& alloc = Allocator())
      : m_allocator{alloc}, m_capacity{n} {
    m_data = m_allocator.allocate(m_capacity);
  }

  constexpr vector(size_type n, const T& value,
                   const Allocator& alloc = Allocator())
      : m_allocator{alloc}, m_capacity{n} {
    // check for available memory to allocate, and default initialize
    // type T on every single position
    if (n == 0) return;
    m_data = m_allocator.allocate(m_capacity);
    try {
      for (; m_size < n; ++m_size) std::construct_at(m_data + m_size, value);
    } catch (...) {
      std::destroy(m_data, m_data + m_size);
      m_allocator.deallocate(m_data, m_capacity);

      // ensure is not in a malformed state
      m_data = nullptr;
      m_capacity = 0;

      // the object dies inmediatly so maybe could skip this
      // resetting, for hygine
      m_size = 0;
      throw;
    }
  };

  // from iterators
  template <std::input_iterator InputIt>
  constexpr vector(InputIt first, InputIt last,
                   const Allocator& alloc = Allocator())
      : m_allocator{alloc} {
    for (; first != last; ++first) push_back(*first);
  };

  // this is compatible with ranges / views
  template <container_compatible_range<T> R>
  constexpr void assign_range(R&& rg) {
    for (auto&& elem : rg) push_back(std::forward<decltype(elem)>(elem));
  };

  constexpr vector(const vector& other) {
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_data = m_allocator.allocate(m_capacity);
    for (size_type i{0}; i < m_size; ++i) m_data[i] = other.m_data[i];
  };

  constexpr vector(vector&& vec) noexcept {
    m_data = vec.m_data;
    m_capacity = vec.m_capacity;
    m_size = vec.m_size;

    vec.m_data = nullptr;
    vec.m_size = 0;
    vec.m_capacity = 0;
  };

  // type_identity_t is used so that the type of the allcoator is preserved
  constexpr vector(const vector&, const type_identity_t<Allocator>&);
  constexpr vector(vector&&, const type_identity_t<Allocator>&);

  constexpr vector(std::initializer_list<T> init,
                   const Allocator& alloc = Allocator())
      : m_allocator{alloc} {
    // we dont do anything if the initializer_list is empty
    if (init.size() == 0) return;
    m_capacity = init.size();
    m_data = m_allocator.allocate(m_capacity);

    try {
      std::uninitialized_copy(init.begin(), init.end(), m_data);
      m_size = m_capacity;
    } catch (...) {
      m_allocator.deallocate(m_data, m_capacity);
      throw;
    }
  };

  constexpr vector& operator=(const vector& other) {
    // dont allow if the sizes of the vectors to
    // copy assign are not equal and when reassigning the copy
    // from the other vector it should not throw

    if (this == &other) return *this;
    m_allocator.deallocate(m_data, m_capacity);
    m_data = m_allocator.allocate(other.m_capacity);
    m_capacity = other.m_capacity;
    m_size = other.m_size;

    for (size_type i{0}; i < m_size; ++i) m_data[i] = other.m_data[i];
    return *this;
  };

  // check what this parts of allocator traits do
  constexpr vector& operator=(vector&& other) noexcept(
      std::allocator_traits<
          Allocator>::propagate_on_container_move_assignment::value ||
      std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this == &other) return *this;
    m_data = other.m_data;
    m_capacity = other.m_capacity;
    m_size = other.m_size;

    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
  };

  constexpr vector& operator=(std::initializer_list<T> init) {
    if (init.size() == 0) return;
    m_capacity = init.size();
    m_data = m_allocator.allocate(m_capacity);

    try {
      std::uninitialized_copy(init.begin(), init.end(), m_data);
      m_size = m_capacity;
    } catch (...) {
      m_allocator.deallocate(m_data, m_capacity);
      throw;
    }

    return *this;
  };

  reference operator[](size_type pos) { return m_data[pos]; }
  const_reference operator[](size_type pos) const { return m_data[pos]; }

  template <std::input_iterator InputIt>
  constexpr void assign(InputIt first, InputIt last);

  constexpr void assign(size_type n, const T& u);
  constexpr void assign(std::initializer_list<T>);

  constexpr void push_back(T&& value) {
    // if the vector is full grow the vector, move the
    // elemnts already inside and allocate the new element,
    // else just allocate the new element
    if (full()) {
      size_type new_capacity =
          capacity() > 0 ? capacity() * m_grow_multiplier : m_new_capacity;
      pointer p{m_allocator.allocate(new_capacity)};

      // move the old storage into the new
      size_type i{0};
      try {
        for (; i < m_size; ++i)
          std::construct_at(p + i, std::move_if_noexcept(m_data[i]));
        std::construct_at(p + m_size, std::move_if_noexcept(value));
      } catch (...) {
        // if while moving them it fails destroy everything and throw
        std::destroy(p, p + i);
        m_allocator.deallocate(p, new_capacity);
        throw;
      }

      // deallocate old storage
      std::destroy(begin(), end());
      m_allocator.deallocate(m_data, m_capacity);

      m_data = p;
      m_capacity = new_capacity;
    } else {
      std::construct_at(m_data + m_size, std::move_if_noexcept(value));
    }
    ++m_size;
  };

  constexpr void push_back(const T& value) {};

  reference at(size_type pos) { return m_data[pos]; }
  const_reference at(size_type pos) const { return m_data[pos]; };

  [[nodiscard]] reference front() { return begin(); };
  [[nodiscard]] const_reference front() const { return cbegin(); }
  [[nodiscard]] reference back() { return rend(); }
  [[nodiscard]] const_reference back() const { return crend(); }

  constexpr T* data() noexcept { return m_data; };
  constexpr T* data() const noexcept { return m_data; };

  constexpr size_type max_size() const noexcept {
    return std::distance(begin(), end());
  }

  [[nodiscard]] constexpr bool is_empty() const noexcept {
    return m_size == 0;
  };
  [[nodiscard]] constexpr size_type size() const noexcept { return m_size; };
  [[nodiscard]] constexpr size_type capacity() const noexcept {
    return m_capacity;
  }

  [[nodiscard]] constexpr bool full() const { return size() == capacity(); }
  void grow() {
    reserve(capacity() > 0 ? capacity() * m_grow_multiplier : m_new_capacity);
  };

  constexpr void reserve(size_type new_cap) {
    // check with allocator_traits if the size passed
    // is within max, maybe return a conditio to check if the
    // reserve was valid
    if (std::allocator_traits<allocator_type>::max_size(m_allocator) < new_cap)
      throw std::length_error("reserve exceddes max cap");

    // if the new capacity is <= to the current capacity do nothing
    if (new_cap <= m_capacity) return;
    T* new_data = m_allocator.allocate(new_cap);
    if (m_size == 0) {
      m_data = new_data;
      m_capacity = new_cap;
      return;
    }

    // is correct capacity to reserve but there are
    // elements on the vector
    size_type i = 0;
    try {
      for (; i < m_size; i++) {
        std::construct_at(new_data + i, std::move_if_noexcept(m_data[i]));
      }
    } catch (const std::exception& e) {
      for (size_t j = 0; j < i; ++j) std::destroy_at(new_data + j);
      m_allocator.deallocate(new_data, new_cap);
      throw e;
    }

    // just destroy them after all have been moved or copied so that
    // we cannot have half constructed - half destructed
    for (size_type i = 0; i < m_size; i++) std::destroy_at(m_data + i);

    m_allocator.deallocate(m_data, m_capacity);
    m_data = new_data;
    m_capacity = new_cap;
  }

  constexpr void shrink_to_fit() {
    // if there is something to shrink allocate size, construct elements on this
    // new size, and destroy eleemnts in current size and deallocate the memory
    // and finally reassign the data_ and the capacity_
    if (m_capacity == m_size) return;
    T* new_data = m_allocator.allocate(m_size);
    size_t i = 0;
    try {
      for (; i < m_size; i++) {
        std::construct_at(new_data + i, std::move_if_noexcept(m_data[i]));
        std::destroy_at(m_data + i);
      }
    } catch (const std::exception& e) {
      // destroy the elements that has been created
      // and deallocate the memory
      for (size_t j = 0; j < i; ++j) std::destroy_at(new_data + j);
      m_allocator.deallocate(new_data, m_size);
      throw e;
    }

    m_allocator.deallocate(m_data, m_size);
    m_data = new_data;
    m_capacity = m_size;
  }

  // we assume that the destructor of the user defined type does not throw
  // otherwise std::terminate
  constexpr void clear() noexcept {
    // can the destructor throw?, when trying to destroy the elements
    for (size_type i = 0; i < m_size; ++i) std::destroy_at(m_data + i);

    // after all have been completely deleted deallocate mem
    m_allocator.deallocate(m_data, m_capacity);
    m_size = 0;
  }

  constexpr iterator insert(const_iterator pos, const T& value) {}
  constexpr iterator insert(const_iterator pos, T&& value) {}
  constexpr iterator insert(const_iterator pos, size_type count,
                            const T& value) {}

  template <typename InputIt>
  constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {}
  constexpr iterator insert(const_iterator pos, std::initializer_list<T> init) {
  }

  template <std::ranges::input_range R>
  constexpr iterator insert_range(const_iterator pos, R&& rg) {}

  template <std::ranges::input_range R>
  constexpr void append_range(R&& rg) {}

  template <typename... Args>
  constexpr iterator emplace(const_iterator pos, Args&&... args) {}

  constexpr iterator erase(const_iterator pos) {}
  constexpr iterator erase(const_iterator first, const_iterator last) {}

  template <typename... Args>
  constexpr reference emplace_back(Args&&... args) {}

  constexpr void pop_back() {
    --m_size;
    std::destroy_at(end());
  };

  void resize(size_type count) {};
  void resize(size_type count, const value_type& value) {}

  constexpr void swap(vector& other) noexcept {
    // if movable swap move them otherwise
    // just copy them
  }

  template <typename I, typename Alloc>
  constexpr void swap(std::vector<T, Alloc>& lhs,
                      std::vector<T, Alloc>& rhs) noexcept {}

  template <typename I, class Alloc, class U>
  constexpr typename std::vector<T, Alloc>::size_type erase(
      std::vector<T, Alloc>& c, const U& value) {};

  template <class I, class Alloc, class Pred>
  constexpr typename std::vector<T, Alloc>::size_type erase_if(
      std::vector<T, Alloc>& c, Pred pred) {};

  // Fix the deduction guide!
  // template <std::ranges::input_range R,
  //           class Alloc = std::allocator<std::ranges::range_value_t<R>>>
  // auto vector(std::from_range_t, R&&, Alloc = Alloc())
  //     -> vector<std::ranges::range_value_t<R>, Alloc>;

  // iterators
  constexpr iterator begin() noexcept { return iterator(m_data); };
  constexpr const_iterator begin() const noexcept {
    return const_iterator(m_data);
  };

  constexpr iterator end() noexcept { return iterator(m_data + m_size); };
  constexpr const_iterator end() const noexcept {
    return iterator(m_data + m_size);
  };
  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(m_data[m_size - 1]);
  };
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(m_data[m_size - 1]);
  };
  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(m_data - 1);
  };
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(m_data - 1);
  };
  constexpr const_iterator cbegin() const noexcept {
    return const_iterator(m_data);
  };
  constexpr const_iterator cend() const noexcept {
    return const_iterator(m_data[m_size]);
  };
  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(m_data[m_size - 1]);
  };
  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(m_data - 1);
  };
  ~vector() { clear(); };

 private:
  pointer m_data{nullptr};
  size_type m_size{0};
  size_type m_capacity{0};

  // Internal allocator defined Allocator
  Allocator m_allocator;

  const int m_grow_multiplier = 2;
  const int m_new_capacity = 16;
};

} // namespace dss

#endif // VECTOR_H
