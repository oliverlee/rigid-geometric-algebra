#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/blade.hpp"

#include <bitset>
#include <compare>
#include <type_traits>

namespace rigid_geometric_algebra {

/// helper type used to define a total order for blade types
/// @tparam A algebra type
///
template <class A>
class blade_ordering
{
  using rep_type = std::bitset<algebra_dimension_v<A>>;
  rep_type value_{};

public:
  template <std::size_t... Is>
  constexpr blade_ordering(std::type_identity<blade<A, Is...>>) noexcept
      : value_{(rep_type{}.set(Is) | ... | rep_type{})}
  {}

  friend constexpr auto operator==(
      const blade_ordering& lhs, const blade_ordering& rhs) noexcept -> bool
  {
    return lhs.value_.to_ullong() == rhs.value_.to_ullong();
  }

  friend constexpr auto
  operator<=>(const blade_ordering& lhs, const blade_ordering& rhs) noexcept
      -> std::weak_ordering
  {
    if (lhs.value_.count() != rhs.value_.count()) {
      return lhs.value_.count() < rhs.value_.count()
                 ? std::weak_ordering::less
                 : std::weak_ordering::greater;
    }

    return lhs.value_.to_ullong() <=> rhs.value_.to_ullong();
  }
};

}  // namespace rigid_geometric_algebra
