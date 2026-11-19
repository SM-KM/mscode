#pragma once

#include <concepts>
#include <functional>

// clang-format off
/// Flyweight 

template<typename T>
concept Internable  = std::constructible_from<T> && std::equality_comparable<T> &&
    requires(const T& t) { { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>; };

