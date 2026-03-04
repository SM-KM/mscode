#include "../vector.h"

#include <gtest/gtest-message.h>
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

  // reserving for vector with existing elements
  dss::vector<int> v;
  v.reserve(2);
  EXPECT_TRUE(v.capacity() == 2);
  v.push_back(5);
  v.push_back(6);
  EXPECT_TRUE(v.size() == 2);

  v.reserve(5);
  EXPECT_TRUE(v.capacity() == 5);
  EXPECT_TRUE(v.size() == 2);
}
