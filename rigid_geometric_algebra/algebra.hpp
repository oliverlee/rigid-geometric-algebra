#pragma once

#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <cstddef>

namespace rigid_geometric_algebra {

/// algebra for a given field type and dimension
/// @tparam F field type
/// @tparam N dimension of Euclidean space
///
/// Models the Clifford algebra Cl_{N,0,1}(F). Provides homogeneous
/// transformations of N-D rigid bodies in (N + 1)-D projective space.
///
/// @see https://en.wikipedia.org/wiki/Plane-based_geometric_algebra
///
template <class F, std::size_t N>
struct algebra
{
  template <std::size_t... Is>
  using blade = ::rigid_geometric_algebra::blade<algebra, Is...>;

  static constexpr auto zero =
      ::rigid_geometric_algebra::zero_constant<algebra>{};
};

}  // namespace rigid_geometric_algebra
