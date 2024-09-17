#pragma once

#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/one.hpp"
#include "rigid_geometric_algebra/scalar_type.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {
namespace detail {

template <class A, class = void>
inline const auto unit_hypervolume = antiscalar_type_t<A>{one<A>};

template <class A>
inline constexpr auto unit_hypervolume<
    A,
    std::enable_if_t<antiscalar_type_t<A>{one<A>}.coefficient == one<A>>> =
    antiscalar_type_t<A>{one<A>};

}  // namespace detail

/// unit (hyper)volume of an algebra
/// @tparam A algebra type
///
/// The unit (hyper)volume is the dual of the unit scalar value in a projective
/// geometric algebra.
/// This implementation defines the unit (hyper)volume element to be positive.
/// @see eq. 2.13
///
template <class A>
  requires is_algebra_v<A>
inline constexpr const auto& unit_hypervolume = detail::unit_hypervolume<A>;

}  // namespace rigid_geometric_algebra
