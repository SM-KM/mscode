#ifndef SET_H
#define SET_H

#include <cstddef>
#include <functional>
#include <iterator>

namespace dss
{
template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key>>
class set
{
  class Iterator
  {
  };

public:
  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using value_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = Allocator::pointer;
  using const_pointer = Allocator::const_pointer;
  using iterator = Iterator;
  using const_iterator = const Iterator;
  using reverse_iterator = std::reverse_iterator<Iterator>;
  using const_reverse_iterator = const std::reverse_iterator<Iterator>;

  // find the node_type specification
  // using node_type
};

class mutliset
{
};

} // namespace dss

#endif // SET_H
