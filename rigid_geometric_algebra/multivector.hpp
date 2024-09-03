#pragma once

#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

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
template <
    class T1,
    class T2,
    class B1 = std::remove_cvref_t<T1>,
    class B2 = std::remove_cvref_t<T2>,
    class A = common_algebra_type_t<B1, B2>>
  requires is_blade_v<B1> and is_blade_v<B2> and
           (not std::is_same_v<typename B1::canonical_type,
                               typename B2::canonical_type>)
// false positive
// NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
constexpr auto operator+(T1&& b1, T2&& b2)
{
  // clang-format off
  return [
    tmp = std::tuple{
      std::forward<T1>(b1).canonical(),
      std::forward<T2>(b2).canonical()
    }  // clang-format on
  ]<template <class...> class list, class... Bs>(list<Bs...>) mutable {
    return multivector{std::get<Bs>(std::move(tmp))...};
  }(sorted_canonical_blades_t<B1, B2>{});
}

// TODO define -: B X B -> V

/// @}

}  // namespace rigid_geometric_algebra
