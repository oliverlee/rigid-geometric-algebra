#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_fwd.hpp"
#include "rigid_geometric_algebra/detail/unique_dimensions.hpp"

#include <cstddef>

namespace rigid_geometric_algebra {

/// basis element of a geometric algebra
/// @tparam A specialization of `algebra`
/// @tparam Is blade index-vector factors
///
/// Defines a basis element of the geometric algebra.
///
/// Every blade has a grade, where the grade the number of factors, equal to
/// `sizeof(Is...)`. The grade of the blade cannot exceed the algebra's
/// projected dimension (`algebra_dimension_v<A> + 1`).
///
/// In traditional vector algebra, a blade with grade 0 is a scalar
/// (`blade<A>`) and a blade with grade 1 is a basis vector (`blade<A, 1>`).
/// `blade<A, 1>{1}` is often expressed as `e1`.
///
/// `blade<A, 1, 2>` is a blade with grade 2 and contains the factors
/// `blade<A, 1>` and `blade<A, 2>`. A grade 2 blade is also called a bivector.
///
/// A blade cannot contain repeated index-vector values. The index-vector values
/// must be less than the algebra's projected dimension. This differs from
/// the standard Lengyel notation which is 1-based and not 0-based; 0
/// corresponds to the projected dimension.
///
/// @see https://terathon.com/foundations_pga_lengyel.pdf
///
template <class A, std::size_t... Is>
  requires ((Is < algebra_dimension_v<A>) and ...) and
           (detail::unique_dimensions(Is...))
struct blade
{

  static constexpr auto grade = sizeof...(Is);

  using value_type = algebra_field_t<A>;

  value_type coefficient{};

  friend auto operator==(const blade&, const blade&) -> bool = default;
};

}  // namespace rigid_geometric_algebra
