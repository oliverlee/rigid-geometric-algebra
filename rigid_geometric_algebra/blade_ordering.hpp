#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/detail/structural_bitset.hpp"

#include <algorithm>
#include <compare>
#include <cstddef>

namespace rigid_geometric_algebra {

/// helper type used to define a total order for blade types
/// @tparam A algebra type
///
template <class A>
struct blade_ordering
{
  using algebra_type = A;
  using mask_type = detail::structural_bitset<algebra_dimension_v<A>>;

  /// mask with bits corresponding to blade dimensions
  ///
  mask_type mask{};

  /// construct with empty mask
  ///
  blade_ordering() = default;

  /// construct from mask
  ///
  constexpr blade_ordering(mask_type mask) : mask{mask} {}

  /// equality operator
  ///
  friend constexpr auto
  operator==(const blade_ordering& lhs, const blade_ordering& rhs) noexcept
      -> bool
  {
    return lhs.mask == rhs.mask;
  }

  /// comparison operator
  ///
  friend constexpr auto
  operator<=>(const blade_ordering& lhs, const blade_ordering& rhs) noexcept
      -> std::strong_ordering
  {
    if (lhs.mask.count() != rhs.mask.count()) {
      return lhs.mask.count() <=> rhs.mask.count();
    }

    return std::lexicographical_compare_three_way(
        rhs.mask.begin(), rhs.mask.end(), lhs.mask.begin(), lhs.mask.end());
  }
};

}  // namespace rigid_geometric_algebra
