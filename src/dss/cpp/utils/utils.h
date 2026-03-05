#ifndef UTILS_H
#define UTILS_H

#include <ranges>

namespace dss {

template <typename T, typename R>
concept container_compatible_range =
    std::ranges::range<T> &&
    std::convertible_to<std::ranges::range_value_t<R>, T>;
}

#endif // UTILS_H
