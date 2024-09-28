#pragma once

#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/decays_to.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

namespace rigid_geometric_algebra::detail {

/// helper type that synthesizes overloads for `zero_constant` arguments
///
/// Provides overloads if `zero_constant` is a function argument. The derived
/// function object must implement a linear transformation - these overloads
/// always return `zero_constant`.
///
class derive_zero_constant_overload
{
public:
  template <class A>
  static constexpr auto operator()(zero_constant<A>) -> zero_constant<A>
  {
    return {};
  }
  template <class T1, class T2>
    requires has_common_algebra_type_v<T1, T2> and
             (detail::decays_to<T1, zero_constant<algebra_type_t<T1>>> or
              detail::decays_to<T2, zero_constant<algebra_type_t<T2>>>)
  static constexpr auto operator()(const T1&, const T2&)
      -> zero_constant<common_algebra_type_t<T1, T2>>
  {
    return {};
  }
};

}  // namespace rigid_geometric_algebra::detail
