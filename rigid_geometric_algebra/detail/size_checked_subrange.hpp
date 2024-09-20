#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"

#include <cstddef>
#include <ranges>

namespace rigid_geometric_algebra::detail {

/// obtain a size-checked subrange
/// @tparam N required size
///
template <std::size_t N, std::ranges::borrowed_range R>
  requires std::ranges::sized_range<R>
// NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
constexpr auto size_checked_subrange(R&& values)
{
  detail::precondition(std::ranges::size(values) == N);

  return std::ranges::subrange{
      std::ranges::begin(values), std::ranges::end(values)};
}

}  // namespace rigid_geometric_algebra::detail
