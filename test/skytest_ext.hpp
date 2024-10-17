#pragma once

#include "skytest/skytest.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>

namespace test {
namespace detail {

template <class T>
concept expandable = requires (T t) { expand(t); };

}  // namespace detail

inline constexpr class
{
public:
  template <class T>
  static constexpr auto operator()(T&& t) -> T&&
  {
    return std::forward<T>(t);
  }
  template <test::detail::expandable T>
  static constexpr auto
  operator()(T&& t) -> decltype(expand(std::forward<T>(t)))
  {
    return expand(std::forward<T>(t));
  }
} expand{};

}  // namespace test

namespace skytest {

inline constexpr auto equal_ranges = pred(std::ranges::equal);

inline constexpr auto equal_elements =
    []<class T, class P = decltype(test::expand)>(
        const T& t1, const T& t2, P proj = {}) {
      return [&]<std::size_t... i>(std::index_sequence<i...>) {
        return (eq(proj(t1[i]), proj(t2[i])) and ...);
      }(std::make_index_sequence<T::size>{});
    };

}  // namespace skytest
