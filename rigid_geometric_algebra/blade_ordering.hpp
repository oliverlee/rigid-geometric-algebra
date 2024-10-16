#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/detail/structural_bitset.hpp"

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

    if (lhs.mask.test(0) != rhs.mask.test(0)) {
      return rhs.mask.test(0) <=> lhs.mask.test(0);
    }

    const auto ml = auto{lhs.mask}.reset(0);
    const auto mr = auto{rhs.mask}.reset(0);

    if (ml.count() == 1) {
      return ml.to_unsigned() <=> mr.to_unsigned();
    }
    // swapping left/right handles the following case
    // e23 < e31 < e12
    // TODO check if this is valid for dimension > 4?
    return mr.to_unsigned() <=> ml.to_unsigned();
  }
};

}  // namespace rigid_geometric_algebra
