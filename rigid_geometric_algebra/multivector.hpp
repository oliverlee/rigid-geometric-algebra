#pragma once

#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"
#include "rigid_geometric_algebra/get_or.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <functional>
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
      : std::tuple<Bs...>{get_or<Bs>(other, Bs{})...}
  {}

  /// checks if a blade is contained in the multivector
  ///
  template <class B>
  static constexpr auto contains = (std::is_same_v<B, Bs> or ...);

  /// addition
  ///
  /// @{

  /// addition of `multivector` types
  /// @tparam V1, V2 cv-ref qualified `multivector` type
  /// @param v1, v2 `multivector` value
  ///
  /// Constructs a multivector containing `blade` elements from `v1` and `v2`.
  /// For each `blade` type `B` in the returned `multivector`, the value is
  /// equal to:
  /// * `get<B>(v1) + get<B>(v2)` if `B` is an element in both `v1` and `v2`;
  ///   otherwise
  /// * `get<B>(v1)` if `B` is an element in `v1`; otherwise
  /// * `get<B>(v2)` if `B` is an element in `v2`
  ///
  template <class V1, class V2>
    requires (std::is_same_v<multivector, std::remove_cvref_t<V1>> or
              std::is_same_v<multivector, std::remove_cvref_t<V2>>) and
             (not detail::is_defined_v<detail::overload<std::plus<>, V1, V2>>)
  friend constexpr auto operator+(V1&& v1, V2&& v2)
  {
    using result_blade_list_type =
        decltype([]<class... T1s, class... T2s>(
                     const std::tuple<T1s...>&, const std::tuple<T2s...>&)
                     -> sorted_canonical_blades_t<T1s..., T2s...> {
          return {};
        }(v1, v2));

    return [&v1, &v2]<class... Ts>(detail::type_list<Ts...>) {
      return multivector<A, Ts...>{(
          get_or<Ts>(std::forward<V1>(v1), zero_constant<A>{}) +
          get_or<Ts>(std::forward<V2>(v2), zero_constant<A>{}))...};
    }(result_blade_list_type{});
  }

  /// @}

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
    -> multivector<algebra_type_t<B0>, B0, Bs...>;

/// addition
///
/// @{

/// addition of  different `blade` types
/// @tparam T1, T2 cv-ref qualified `blade` type
/// @param b1, b2 `blade` value
///
/// Constructs a multivector containing the canonical form of `b1` and `b2`.
///
template <
    class T1,
    class T2,
    class B1 = std::remove_cvref_t<T1>,
    class B2 = std::remove_cvref_t<T2>,
    class A = common_algebra_type_t<B1, B2>>
  requires is_blade_v<B1> and is_blade_v<B2> and
           (not std::is_same_v<canonical_type_t<B1>, canonical_type_t<B2>>)
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
