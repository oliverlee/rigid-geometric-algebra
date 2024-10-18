#pragma once

#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/field_identity.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"

#include <functional>
namespace rigid_geometric_algebra {

/// specifies the multiplicative identity value
/// @tparam A algebra type
///
/// Reference to the multiplicative identity value.
///
/// For values `x` in the valid domain of `algebra_field_t<A>`, the following
/// expressions are true:
/// * `x * one<A> == x`
/// * `one<A> * x == x`
///
template <class A>
  requires is_algebra_v<A>
inline constexpr const auto& one =
    field_identity<algebra_field_t<A>, std::multiplies<>>;

}  // namespace rigid_geometric_algebra
