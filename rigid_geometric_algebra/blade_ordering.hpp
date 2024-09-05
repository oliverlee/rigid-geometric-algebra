#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/detail/structural_bitset.hpp"

#include <compare>
#include <cstddef>
#include <type_traits>

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

  /// construct a `blade_ordering` from a blade type
  ///
  template <std::size_t... Is>
  constexpr blade_ordering(std::type_identity<blade<A, Is...>>) noexcept
      : mask{(mask_type{}.set(Is) | ... | mask_type{})}
  {}

  /// equality operator
  ///
  friend constexpr auto operator==(
      const blade_ordering& lhs, const blade_ordering& rhs) noexcept -> bool
  {
    return lhs.mask == rhs.mask;
  }

  /// comparison operator
  ///
  friend constexpr auto
  operator<=>(const blade_ordering& lhs, const blade_ordering& rhs) noexcept
      -> std::weak_ordering
  {
    if (lhs.mask.count() != rhs.mask.count()) {
      return lhs.mask.count() < rhs.mask.count()
                 ? std::weak_ordering::less
                 : std::weak_ordering::greater;
    }

    return lhs.mask.to_unsigned() <=> rhs.mask.to_unsigned();
  }
};

}  // namespace rigid_geometric_algebra
