#pragma once

#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/field.hpp"
#include "rigid_geometric_algebra/multivector.hpp"
#include "rigid_geometric_algebra/point.hpp"
#include "rigid_geometric_algebra/scalar_type.hpp"
#include "rigid_geometric_algebra/unit_hypervolume.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <cstddef>

namespace rigid_geometric_algebra {

/// geometric algebra for a given field type and dimension
/// @tparam F field type
/// @tparam N dimension of Euclidean space
///
/// Models the Clifford algebra Cl_{N,0,1}(F). Provides homogeneous
/// transformations of N-D rigid bodies in (N + 1)-D projective space.
///
/// @see https://en.wikipedia.org/wiki/Plane-based_geometric_algebra
///
template <field F, std::size_t N>
struct algebra
{
  static constexpr auto zero =
      ::rigid_geometric_algebra::zero_constant<algebra>{};

  template <std::size_t... Is>
  using blade = ::rigid_geometric_algebra::blade<algebra, Is...>;

  template <class... Bs>
  using multivector = ::rigid_geometric_algebra::multivector<algebra, Bs...>;

  using point = ::rigid_geometric_algebra::point<algebra>;

  using scalar = ::rigid_geometric_algebra::scalar_type_t<algebra>;

  using antiscalar = ::rigid_geometric_algebra::antiscalar_type_t<algebra>;

  static constexpr const auto& unit_hypervolume =
      ::rigid_geometric_algebra::unit_hypervolume<algebra>;
};

}  // namespace rigid_geometric_algebra
