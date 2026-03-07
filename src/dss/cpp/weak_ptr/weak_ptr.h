#ifndef WEAK_PTR_H
#define WEAK_PTR_H

#include <type_traits>

#include "../shared_ptr/shared_ptr.h"

namespace dss {

template <typename T>
class weak_ptr {
  // is to remove the first dimension of an array if is
  // not an array it remains unchanged
  using element_type = std::remove_extent_t<T>;

  constexpr weak_ptr() noexcept;
  weak_ptr(const weak_ptr& r) noexcept;
  template <typename Y>
  weak_ptr(const weak_ptr<Y>& r) noexcept;
  template <typename Y>
  weak_ptr(const shared_ptr<Y>& r) noexcept;
  weak_ptr(weak_ptr&& r) noexcept;
  template <typename Y>
  weak_ptr(weak_ptr<Y>&& r) noexcept;

  ~weak_ptr();

  weak_ptr& operator=(const weak_ptr& r) noexcept;
  template <typename Y>
  weak_ptr& operator=(const weak_ptr<Y>& r) noexcept;
  template <typename Y>
  weak_ptr& operator=(const shared_ptr<Y>& r) noexcept;
  weak_ptr& operator=(weak_ptr&& r) noexcept;
  template <typename Y>
  weak_ptr& operator=(weak_ptr<Y>&& r) noexcept;

  void reset() noexcept;
  void swap(weak_ptr& r) noexcept;
  long use_count() const noexcept;
  bool expired() const noexcept;
  std::shared_ptr<T> lock() const noexcept;

  template <typename Y>
  bool owner_before(const weak_ptr<Y>& other) const noexcept;
  template <typename Y>
  bool owner_before(const std::shared_ptr<Y>& other) const noexcept;

  std::size_t owner_hash() const noexcept;

  template <typename Y>
  bool owner_equal(const std::weak_ptr<Y>& other) const noexcept;
  template <typename Y>
  bool owner_equal(const std::shared_ptr<Y>& other) const noexcept;
};

template <typename T>
weak_ptr(shared_ptr<T>) -> weak_ptr<T>;

} // namespace dss

#endif // WEAK_PTR_H
