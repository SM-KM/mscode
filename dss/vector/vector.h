#ifndef VECTOR_H
#define VECTOR_H

#include <alloca.h>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>

namespace dss {
template <typename T, typename Allocator = std::allocator<T>> class vector {
  class iterator {};

public:
  using value_type = T;

  template <typename U> using type_identity_t = U;
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

  // the allocator constructor has to be no expect as well
  constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {};
  constexpr explicit vector(const Allocator&) noexcept;
  constexpr explicit vector(size_type n, const Allocator& = Allocator());

  constexpr vector(size_type n, const T& value, const Allocator& = Allocator());

  // from iterators
  template <typename InputIt>
  constexpr vector(InputIt first, InputIt last, const Allocator& = Allocator());

  // template<container-compatible-range<T> R>
  // constexpr void assign_range(R&& rg);

  constexpr vector(const vector& x);
  constexpr vector(vector&&) noexcept;
  // type_identity_t is used so that the type of the allcoator is preserved
  constexpr vector(const vector&, const type_identity_t<Allocator>&);
  constexpr vector(vector&&, const type_identity_t<Allocator>&);
  constexpr vector(std::initializer_list<T>, const Allocator& = Allocator());
  ~vector();

  constexpr vector& operator=(const vector& x);

  // check what this parts of allocator traits do
  constexpr vector& operator=(vector&& x) noexcept(
      std::allocator_traits<
          Allocator>::propagate_on_container_move_assignment::value ||
      std::allocator_traits<Allocator>::is_always_equal::value);

  constexpr vector& operator=(std::initializer_list<T>);

  template <typename InputIt>
  constexpr void assign(InputIt first, InputIt last);
  constexpr void assign(size_type n, const T& u);
  constexpr void assign(std::initializer_list<T>);

  // iterators
  constexpr Allocator getAllocator() const noexcept;
  constexpr iterator begin() noexcept;
  constexpr const_iterator begin() const noexcept;
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

  [[nodiscard]] constexpr bool isEmpty() const noexcept;
  constexpr size_type size() const noexcept;
};

} // namespace dss

#endif // VECTOR_H
