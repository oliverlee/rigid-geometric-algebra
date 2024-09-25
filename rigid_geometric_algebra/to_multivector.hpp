#pragma once

#include "algebra_type.hpp"
#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/multivector.hpp"

#include <utility>

namespace rigid_geometric_algebra {

/// promote value to a `multivector`
/// @tparam B cv-ref qualified `blade` type
/// @tparam b `blade` value
///
/// Returns a `multivector` containing a single `blade`.
///
template <detail::blade B>
constexpr auto
to_multivector(B&& b) -> multivector<algebra_type_t<B>, canonical_type_t<B>>
{
  return {std::forward<B>(b).canonical()};
}

}  // namespace rigid_geometric_algebra
