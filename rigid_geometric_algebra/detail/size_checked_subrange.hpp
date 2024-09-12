#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <ranges>

namespace rigid_geometric_algebra::detail {

/// obtain a size-checked subrange
/// @tparam N required size
///
template <std::size_t N>
constexpr auto size_checked_subrange(std::ranges::sized_range auto values)
{
  if (std::ranges::size(values) != N) {
    assert(false and "invalid size");
    std::abort();
  }

  return std::ranges::subrange{
      std::ranges::begin(values), std::ranges::end(values)};
}

}  // namespace rigid_geometric_algebra::detail
