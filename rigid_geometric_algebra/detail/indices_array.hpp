#pragma once

#include "rigid_geometric_algebra/detail/contract.hpp"
#include "rigid_geometric_algebra/detail/indices_of.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>

namespace rigid_geometric_algebra::detail {

/// create an array containing the indices of a mask
///
template <auto mask, auto pred = std::identity{}>
inline constexpr auto indices_array = [] {
  auto indices = std::array<std::size_t, std::ranges::count_if(mask, pred)>{};

  auto it = detail::indices_of(mask, indices.begin());
  detail::postcondition(it == indices.end());

  return indices;
}();

}  // namespace rigid_geometric_algebra::detail
