#include "../vector.h"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

TEST(vector, constructor) {
  dss::vector<int> vec;
  EXPECT_TRUE(vec.size() == 0);
  EXPECT_TRUE(vec.capacity() == 0);
}

TEST(vector, size_value_constructor) {
  dss::vector<int> vec(5, 5);
  EXPECT_TRUE(vec.size() == 5);
  EXPECT_TRUE(vec.capacity() == 5);
  for (size_t i = 0; i < vec.size(); ++i) EXPECT_TRUE(vec[i] == 5);
}

TEST(vector, capacity_constructor) {
  dss::vector<int> vec(10);
  EXPECT_TRUE(vec.capacity() == 10);
  EXPECT_TRUE(vec.size() == 0);
}

TEST(vector, initializer_list_constructor) {
  dss::vector<int> vec = {1, 2, 5};
  EXPECT_TRUE(vec.size() == 3);
  EXPECT_TRUE(vec.capacity() == 3);
}

TEST(vector, push_back) {
  EXPECT_TRUE(true);
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

TEST(vector, shrink_to_fit) {
  dss::vector<int> v;
  EXPECT_TRUE(v.size() == 0);
  EXPECT_TRUE(v.capacity() == 0);
  v.shrink_to_fit();
  EXPECT_TRUE(v.size() == 0);
  EXPECT_TRUE(v.capacity() == 0);

  // when ther is elements
  v.push_back(20);
  EXPECT_TRUE(v.capacity() == 16);
  EXPECT_TRUE(v.size() == 1);
  v.shrink_to_fit();
  EXPECT_TRUE(v.capacity() == 1);
  EXPECT_TRUE(v.size() == 1);
}
