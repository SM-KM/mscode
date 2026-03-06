#ifndef MEMORY_H
#define MEMORY_H

#include <iterator>
namespace dss {

template <typename T>
constexpr T* addressof(T& arg) noexcept;
template <class T>
const T* addressof(const T&&) = delete;

template <std::input_iterator InputIt, std::input_iterator NoThrowForwardIt>
NoThrowForwardIt uninitialized_move(InputIt first, InputIt last,
                                    NoThrowForwardIt d_first);
template <typename ExecutionPolicy, typename ForwardIt,
          typename NoThrowForwardIt>
NoThrowForwardIt uninitialized_move(ExecutionPolicy&& policy, ForwardIt first,
                                    ForwardIt last, NoThrowForwardIt d_first);

template <typename ExecutionPolicy, typename ForwardIt,
          typename NoThrowForwardIt>
NoThrowForwardIt uninitialized_copy(ExecutionPolicy&& policy, ForwardIt first,
                                    ForwardIt last, NoThrowForwardIt d_first);

template <typename T, typename... Args>
constexpr T* construct_at(T* p, Args&&... args);

template <class T>
constexpr void destroy_at(T* p);

template <class ForwardIt>
constexpr void destroy(ForwardIt first, ForwardIt last);
template <class ExecutionPolicy, class ForwardIt>
void destroy(ExecutionPolicy&& policy, ForwardIt first, ForwardIt last);

} // namespace dss

#endif // MEMORY_H
