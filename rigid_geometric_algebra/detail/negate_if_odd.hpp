#pragma once

#include "rigid_geometric_algebra/detail/even.hpp"

#include <cstddef>
#include <functional>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// function object that is either negation or identity
/// @tparam N number
///
/// Unary function object that returns the argument negated if `N` is odd.
/// Otherwise it returns the argument unchanged if `N` is even. This is
/// typically used to determine if a blade coefficient needs to be negated
/// after performing a wedge operation. In this case, `N` is typically the
/// number of swaps of dimensions needed for the canonical of the blade.
///
template <std::size_t N>
using negate_if_odd =
    std::conditional_t<detail::even(N), std::identity, std::negate<>>;

}  // namespace rigid_geometric_algebra::detail
