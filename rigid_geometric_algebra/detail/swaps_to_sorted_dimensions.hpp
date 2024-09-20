#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <functional>
#include <ranges>

namespace rigid_geometric_algebra::detail {

inline constexpr class
{
  template <class R1, class R2>
  static constexpr auto impl(R1 unsorted, R2 sorted) -> std::size_t
  {
    auto count = 0UZ;

    for (; unsorted.size() > 1; unsorted.advance(1), sorted.advance(1)) {
      auto it = std::ranges::find(unsorted, sorted.front());

      // if elements are adjacent
      // a ^ b
      // this is equivalent to
      // -(b ^ a)
      // and swapping the order increases the negation count
      //
      // else if element are not adjacent
      // a ^ (...) ^ b
      // this is equivalent to
      // a ^ b ^ (...) * (-1)^n         -- +n swaps
      // b ^ a ^ (...) * (-1)^(n + 1)   -- +1 swap
      // b ^ (...) ^ a * (-1)^(2*n + 1) -- +n swaps
      // and the even number of swaps (2*n) get "canceled" out

      if (it != unsorted.begin()) {
        ++count;
      }
      std::iter_swap(it, unsorted.begin());
    }

    return count;
  }

public:
  template <std::same_as<std::size_t>... Ts>
  constexpr static auto operator()(Ts... is) -> std::size_t
  {
    auto unsorted = std::array<std::size_t, sizeof...(is)>{is...};

    const auto sorted = [&unsorted] {
      auto data = unsorted;
      std::ranges::sort(data);
      detail::precondition(
          std::ranges::unique(data).empty(), "values must be unique");
      return data;
    }();

    detail::postcondition(
        std::ranges::is_sorted(sorted, std::ranges::less_equal{}));
    return impl(std::ranges::subrange(unsorted), std::ranges::subrange(sorted));
  }

} swaps_to_sorted_dimensions{};

}  // namespace rigid_geometric_algebra::detail
