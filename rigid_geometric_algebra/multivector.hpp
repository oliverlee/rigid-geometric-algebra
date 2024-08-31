#pragma once

#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"

#include <tuple>
#include <type_traits>

namespace rigid_geometric_algebra {

// forward declaration
//
namespace detail {
template <class>
class get_or_fn;
}

/// linear combination of basis elements of a geometric algebra
/// @tparam A specialization of `algebra`
/// @tparam Bs blades
///
/// @see https://terathon.com/foundations_pga_lengyel.pdf
///
template <class A, class... Bs>
  requires is_algebra_v<A> and (is_canonical_blade_order<Bs...>()) and
           std::is_same_v<A, common_algebra_type_t<Bs...>>
struct multivector : std::tuple<Bs...>
{
  /// algebra this blade belongs to
  ///
  using algebra_type = A;

  /// blade scalar type
  ///
  using value_type = algebra_field_t<A>;

  /// inherited constructors from std::tuple
  ///
  using std::tuple<Bs...>::tuple;

  /// non-narrowing construction from a different multivector
  ///
  template <class... Cs>
  constexpr explicit multivector(const multivector<Cs...>& other)
      : std::tuple<Bs...>{detail::get_or_fn<Bs>{}(other, Bs{})...}
  {}

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

/// add different blade types to form a multivector
///
// TODO allow either B1/B2 order
template <class B1, class B2>
  requires is_blade_v<std::remove_cvref_t<B1>> and
           is_blade_v<std::remove_cvref_t<B2>> and
           std::is_same_v<typename std::remove_cvref_t<B1>::algebra_type,
                          typename std::remove_cvref_t<B2>::algebra_type> and
           (not std::is_same_v<
               typename std::remove_cvref_t<B1>::canonical_type,
               typename std::remove_cvref_t<B2>::canonical_type>)
constexpr auto operator+(B1&& b1, B2&& b2)
{
  // NOTE hard error if specified as return type
  // but this needs to handled by allowing determining B1/B2 order
  return multivector<
      typename std::remove_cvref_t<B1>::algebra_type,
      std::remove_cvref_t<B1>,
      std::remove_cvref_t<B2>>{std::forward<B1>(b1), std::forward<B2>(b2)};
}

// TODO define -: B X B -> V

/// @}

}  // namespace rigid_geometric_algebra
