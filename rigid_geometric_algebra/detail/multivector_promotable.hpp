#pragma once

#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"

namespace rigid_geometric_algebra::detail {

/// implementation-only concept to determine if a type is promotable to a
/// `multivector`
///
template <class T>
concept multivector_promotable = detail::blade<T> or detail::multivector<T>;

}  // namespace rigid_geometric_algebra::detail
