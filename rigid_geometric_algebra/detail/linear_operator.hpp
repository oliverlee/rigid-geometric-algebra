#pragma once

#include "rigid_geometric_algebra/detail/derive_multivector_overload.hpp"
#include "rigid_geometric_algebra/detail/derive_zero_constant_overload.hpp"
namespace rigid_geometric_algebra::detail {

/// defines a linear operator from `blade` overloads
/// @tparam F function object defining `blade` overloads
///
/// Defines a function object with overloads for `zero_constant`, `blade`, and
/// `multivector` arguments.
///
template <class F>
class linear_operator
    : detail::derive_zero_constant_overload,
      F,
      detail::derive_multivector_overload<F>
{
public:
  using detail::derive_zero_constant_overload::operator();
  using F::operator();
  using detail::derive_multivector_overload<F>::operator();
};

}  // namespace rigid_geometric_algebra::detail
