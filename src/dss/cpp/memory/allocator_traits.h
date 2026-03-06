#ifndef ALLOCATOR_TRAITS_H
#define ALLOCATOR_TRAITS_H

namespace dss {

template <typename Alloc>
struct allocator_traits {
  using allocator_type = Alloc;
  using value_type = Alloc::value_type;
  using pointer = Alloc::pointer;
};

} // namespace dss

#endif // ALLOCATOR_TRAITS_H
