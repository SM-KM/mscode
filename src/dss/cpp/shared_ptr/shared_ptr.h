#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <cstddef>
#include <memory>

template <typename T>
class shared_ptr
{
public:
  using element_type = T;
  using weak_type = std::weak_ptr<T>;

  shared_ptr(shared_ptr<T>&& r, element_type *ptr) noexcept;
  shared_ptr& operator=(const shared_ptr& r) noexcept;

  void reset() noexcept {};
  template <typename Y>
  void reset(Y *ptr) {};
  template <typename Y, typename Deleter>
  void reset(Y *ptr, Deleter d) {};
  template <typename Y, typename Deleter, typename Alloc>
  void reset(Y *ptr, Deleter d, Alloc alloc) {};

  void swap(shared_ptr& r) noexcept {};
  element_type *get() const noexcept {};

  // operators
  T& operator*() const noexcept {};
  T *operator->() const noexcept {};
  element_type& operator[](std::ptrdiff_t idx) const;
  explicit operator bool() noexcept {};

  long use_count() const noexcept;

  template <typename Y>
  bool owner_before(const shared_ptr<Y>& other) const noexcept;
  template <typename Y>
  bool owner_before(const std::weak_ptr<Y>& other) const noexcept;

  ~shared_ptr();
};

#endif // SHARED_PTR_H
