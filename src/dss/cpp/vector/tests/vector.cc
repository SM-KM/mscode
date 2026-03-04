#include "../vector.h"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

TEST(vector, constructor) {
  dss::vector<int> vec;
  EXPECT_TRUE(vec.size() == 0);
  EXPECT_TRUE(vec.capacity() == 0);
}

TEST(vector, reserve) {
  dss::vector<int> vec;
  auto alloc = vec.get_allocator();
  EXPECT_THROW(
      vec.reserve(
          std::allocator_traits<dss::vector<int>::allocator_type>::max_size(
              alloc) +
          1),
      std::length_error);

  int new_capacity = 20;
  EXPECT_TRUE(vec.capacity() == 0);
  vec.reserve(new_capacity);
  EXPECT_TRUE(vec.capacity() == new_capacity);
}
