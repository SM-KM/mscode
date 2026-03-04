#include "../vector.h"

#include <gtest/gtest.h>

TEST(vector, constructor) {
  dss::vector<int> vec;
  EXPECT_TRUE(vec.size() == 0);
  EXPECT_TRUE(vec.capacity() == 0);
}
