#ifndef VECTOR_H
#define VECTOR_H

#include <alloca.h>
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ranges>
#include <type_traits>
#include <vector>

namespace dss
{
template <typename T, typename Allocator = std::allocator<T>>
class vector
{
  // considerations i want reference
  // access to iterators
  class iterator
  {
    using iterator_category = std::contiguous_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = T&;
    using pointer = T *;

  public:
    T *ptr;
    vector vec;

    // Constructors
    constexpr iterator() = default;
    constexpr explicit iterator(vector& vec_) : vec{vec_} {}

    // Operators
    constexpr T& operator*() { return *ptr; }
    constexpr bool operator==(const iterator& other) const
    {
      return other.ptr == ptr;
    }

    constexpr iterator& operator++()
    {
      ++ptr;
      return *this;
    }

    constexpr iterator operator++(int)
    {
      auto prev = *this;
      ++(*this);
      return prev;
    }

    constexpr iterator& operator--()
    {
      --ptr;
      return *this;
    }

    constexpr iterator operator--(int)
    {
      auto prev = *this;
      ++(*this);
      return prev;
    }

    constexpr iterator& operator+=(difference_type n)
    {
      ptr += n;
      return *this;
    }

    constexpr iterator operator+(difference_type n) const
    {
      auto next = *this;
      next += n;
      return next;
    }

    constexpr iterator& operator-=(difference_type n)
    {
      ptr -= n;
      return *this;
    }

    constexpr iterator operator-(difference_type n) const
    {
      auto next = *this;
      next -= n;
      return next;
    }

    difference_type operator-(const iterator& other) const
    {
      return ptr - other.ptr;
    }
    constexpr T& operator[](difference_type n) { return ptr[n]; }
    constexpr auto operator<=>(const iterator& other)
    {
      return other.ptr <=> ptr;
    }
    constexpr T *operator->() const { return ptr->data(); }
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
  using iterator = iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using allocator_type = Allocator;

  constexpr allocator_type get_allocator() const noexcept
  {
    return m_allocator;
  };

  // the allocator constructor has to be no expect as well
  constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {};
  constexpr explicit vector(const Allocator& alloc) noexcept
      : m_allocator{alloc} {};
  constexpr explicit vector(size_type n, const Allocator& alloc = Allocator())
      : m_allocator{alloc}, m_size{n}
  {
  }
  constexpr vector(size_type n, const T& value,
                   const Allocator& alloc = Allocator())
      : m_allocator{alloc}, m_size{n} {};

  // from iterators
  template <typename InputIt>
  constexpr vector(InputIt first, InputIt last,
                   const Allocator& alloc = Allocator())
      : m_allocator{alloc} {};

  // template<container-compatible-range<T> R>
  // constexpr void assign_range(R&& rg);
  constexpr vector(const vector& x) {};
  constexpr vector(vector&& vec) noexcept {};

  // type_identity_t is used so that the type of the allcoator is preserved
  constexpr vector(const vector&, const type_identity_t<Allocator>&);
  constexpr vector(vector&&, const type_identity_t<Allocator>&);
  constexpr vector(std::initializer_list<T> init,
                   const Allocator& alloc = Allocator())
      : m_allocator{alloc}
  {
    m_size = init.size();
    m_capacity = m_size;
    m_data = m_allocator.allocate(m_size);
    std::uninitialized_copy(init.begin(), init.end(), m_data);
  };

  constexpr vector& operator=(const vector& x);

  // check what this parts of allocator traits do
  constexpr vector& operator=(vector&& x) noexcept(
      std::allocator_traits<
          Allocator>::propagate_on_container_move_assignment::value ||
      std::allocator_traits<Allocator>::is_always_equal::value);

  constexpr vector& operator=(std::initializer_list<T> init) {};

  reference operator[](size_type pos) {}
  const_reference operator[](size_type pos) const {}

  template <typename InputIt>
  constexpr void assign(InputIt first, InputIt last);
  constexpr void assign(size_type n, const T& u);
  constexpr void assign(std::initializer_list<T>);

  constexpr void push_back(T&& value) {};
  constexpr void push_back(const T& value) {};

  reference at(size_type pos) { return m_data[pos]; }
  const_reference at(size_type pos) const { return m_data[pos]; };

  reference front() {};
  const_reference front() const {}

  reference back() {}
  const_reference back() const {}

  constexpr T *data() noexcept {};
  constexpr T *data() const noexcept {};

  constexpr size_type max_size() const noexcept
  {
    return std::distance(begin(), end());
  }

  [[nodiscard]] constexpr bool is_empty() const noexcept
  {
    return m_size == 0;
  };
  [[nodiscard]] constexpr size_type size() const noexcept { return m_size; };
  [[nodiscard]] constexpr size_type capacity() const noexcept
  {
    return m_capacity;
  }

  [[nodiscard]] constexpr bool full() const { return size() == capacity(); }
  void grow()
  {
    reserve(capacity() > 0 ? capacity() * m_grow_multiplier : m_new_capacity);
  };

  constexpr void reserve(size_type new_cap) {}
  constexpr void shrink_to_fit() {}
  constexpr void clear() noexcept {}

  constexpr iterator insert(const_iterator pos, const T& value) {}
  constexpr iterator insert(const_iterator pos, T&& value) {}
  constexpr iterator insert(const_iterator pos, size_type count, const T& value)
  {
  }

  template <typename InputIt>
  constexpr iterator insert(const_iterator pos, InputIt first, InputIt last)
  {
  }

  constexpr iterator insert(const_iterator pos, std::initializer_list<T> init)
  {
  }

  template <std::ranges::input_range R>
  constexpr iterator insert_range(const_iterator pos, R&& rg)
  {
  }

  template <std::ranges::input_range R>
  constexpr void append_range(R&& rg)
  {
  }

  template <typename... Args>
  constexpr iterator emplace(const_iterator pos, Args&&...args)
  {
  }

  constexpr iterator erase(const_iterator pos) {}
  constexpr iterator erase(const_iterator first, const_iterator last) {}

  template <typename... Args>
  constexpr reference emplace_back(Args&&...args)
  {
  }

  constexpr void pop_back() {};
  void resize(size_type count) {};
  void resize(size_type count, const value_type& value) {}

  constexpr void swap(vector& other) noexcept {}

  template <typename I, typename Alloc>
  constexpr void swap(std::vector<T, Alloc>& lhs,
                      std::vector<T, Alloc>& rhs) noexcept
  {
  }

  template <typename I, class Alloc, class U>
  constexpr typename std::vector<T, Alloc>::size_type
  erase(std::vector<T, Alloc>& c, const U& value) {};

  template <class I, class Alloc, class Pred>
  constexpr typename std::vector<T, Alloc>::size_type
  erase_if(std::vector<T, Alloc>& c, Pred pred) {};

  // Fix the deduction guide!
  // template <std::ranges::input_range R,
  //           class Alloc = std::allocator<std::ranges::range_value_t<R>>>
  // vector(std::from_range_t, R&&, Alloc = Alloc())
  //     -> vector<std::ranges::range_value_t<R>, Alloc>;

  // iterators
  constexpr Allocator getAllocator() const noexcept { return m_allocator; };
  constexpr iterator begin() noexcept { return iterator(m_data); };
  constexpr const_iterator begin() const noexcept
  {
    return const_iterator(m_data);
  };

  constexpr iterator end() noexcept;
  constexpr const_iterator end() const noexcept;
  constexpr reverse_iterator rbegin() noexcept;
  constexpr const_reverse_iterator rbegin() const noexcept;
  constexpr reverse_iterator rend() noexcept;
  constexpr const_reverse_iterator rend() const noexcept;

  constexpr const_iterator cbegin() const noexcept;
  constexpr const_iterator cend() const noexcept;
  constexpr const_reverse_iterator crbegin() const noexcept;
  constexpr const_reverse_iterator crend() const noexcept;

  // Helpers
  void cleanup_on_fail_aux(iterator last)
  {
    auto p{begin()};
    for (; p != last; ++p)
      std::destroy_at(p.ptr);
    delete (m_data);
  }

  template <typename U>
    requires std::is_convertible_v<U, T>
  void copy_aux(const U& init, size_type size)
  {
    auto i{begin()};
    try
    {
      for (; i != begin() + size; ++i)
        std::construct_at(i.ptr, init);
    }
    catch (const std::exception&)
    {
      cleanup_on_fail_aux(i);
      throw;
    }
  }

  // take in consideration that if for example the last one fails, it would
  // destroy everything before the element that fails
  template <typename It>
    requires std::forward_iterator<It>
  void copy_rng_aux(It first, It last)
  {
    auto i{begin()};
    auto j{first};
    try
    {
      for (; j != last; ++i, ++j)
        std::construct_at(i.ptr, *j);
    }
    catch (const std::exception&)
    {
      // in the case that it fails the copy does not happen but the user does
      // not know about it
      cleanup_on_fail_aux(i);
    }
  }

  // use instead std::allocator<T>, maybe do it to a variable for debugging and
  // logging support with policies
  pointer allocate_aux(size_type new_capacity) { return new T[new_capacity]; }
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
