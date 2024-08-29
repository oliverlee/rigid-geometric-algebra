#pragma once

#include "rigid_geometric_algebra/detail/unique_types.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"

#include <tuple>
#include <type_traits>

namespace rigid_geometric_algebra {

/// linear combination of basis elements of a geometric algebra
/// @tparam A specialization of `algebra`
/// @tparam Bs blades
///
/// @see https://terathon.com/foundations_pga_lengyel.pdf
///
template <class A, class... Bs>
  requires is_algebra_v<A> and ((is_blade_v<Bs>) and ...) and
           (std::is_same_v<A, typename Bs::algebra_type> and ...) and
           (detail::unique_types<Bs...>())
struct multivector : std::tuple<Bs...>
{
  /// inherited constructors from std::tuple
  ///
  using std::tuple<Bs...>::tuple;

  /// checks if a blade is contained in the multivector
  ///
  template <class B>
  static constexpr auto contains = (std::is_same_v<B, Bs> or ...);

  /// equality comparison
  ///
  /// @{

  friend auto
  operator==(const multivector&, const multivector&) -> bool = default;

  /// @}
};

/// deduction guide
///
template <class B0, class... Bs>
multivector(const B0&, const Bs&...)
    -> multivector<typename B0::algebra_type, B0, Bs...>;

/// addition
///
/// @{

template <class B1, class B2>
  requires std::is_same_v<typename B1::algebra_type, typename B2::algebra_type>
constexpr auto operator+(const B1& b1, const B2& b2)
    -> multivector<typename B1::algebra_type, B1, B2>
{
  return {b1, b2};
}

/// @}

}  // namespace rigid_geometric_algebra
