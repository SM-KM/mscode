#ifndef UTILS_H
#define UTILS_H

#include <ranges>

namespace dss {

template <typename T, typename R>
concept container_compatible_range =
    std::ranges::range<T> &&
    std::convertible_to<std::ranges::range_value_t<R>, T>;
}

#define GCC_VERSION \
  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#endif // UTILS_H
