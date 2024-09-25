#pragma once

#include "detail/overload.hpp"
#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/derive_subtraction.hpp"
#include "rigid_geometric_algebra/detail/derive_vector_space_operations.hpp"
#include "rigid_geometric_algebra/detail/is_defined.hpp"
#include "rigid_geometric_algebra/detail/rebind_args_into.hpp"
#include "rigid_geometric_algebra/detail/size_checked_subrange.hpp"
#include "rigid_geometric_algebra/detail/type_concat.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"
#include "rigid_geometric_algebra/get.hpp"
#include "rigid_geometric_algebra/get_or.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"
#include "rigid_geometric_algebra/wedge.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {

/// linear combination of basis elements of a geometric algebra
/// @tparam A specialization of `algebra`
/// @tparam Bs blades
///
/// @see https://terathon.com/foundations_pga_lengyel.pdf
///
template <class A, class... Bs>
  requires is_algebra_v<A> and (is_canonical_blade_order<Bs...>()) and
               (std::is_same_v<A, algebra_type_t<Bs>> and ...)
class multivector
    : public std::tuple<Bs...>,
      detail::derive_vector_space_operations<
          multivector<A, Bs...>,
          detail::get_fn<Bs>...>,
      detail::derive_subtraction<multivector<A, Bs...>>
{
public:
  /// algebra this blade belongs to
  ///
  using algebra_type = A;

  /// blade scalar type
  ///
  using value_type = algebra_field_t<A>;

  /// number of blades in this multivector
  ///
  static constexpr auto size =
      std::integral_constant<std::size_t, sizeof...(Bs)>{};

  /// inherited constructors from std::tuple
  ///
  using std::tuple<Bs...>::tuple;

  /// blade list
  ///
  using blade_list_type = std::tuple<Bs...>;

  /// non-narrowing construction from a different multivector
  ///
  template <class... Cs>
  constexpr explicit multivector(const multivector<Cs...>& other)
      : std::tuple<Bs...>{get_or<Bs>(other, Bs{})...}
  {}

  /// initializer list constructor
  /// @param il initializer list of values
  ///
  /// @note allows conversions to floating point `value_type`s and if the
  ///   source can be stored exactly
  /// @see
  /// https://en.cppreference.com/w/cpp/language/list_initialization#Narrowing_conversions
  ///
  constexpr multivector(std::initializer_list<value_type> il)
    requires std::floating_point<value_type>
      : std::tuple<Bs...>{
            []<std::size_t... Is>(std::index_sequence<Is...>, auto values) {
              return std::tuple<Bs...>{values[Is]...};
            }(std::index_sequence_for<Bs...>{},
              detail::size_checked_subrange<sizeof...(Bs)>(il))}
  {}

  /// checks if a blade is contained in the multivector
  ///
  template <class B>
  static constexpr auto contains = (std::is_same_v<B, Bs> or ...);

  /// promote value to a `multivector`
  /// @tparam Self `multivector` type
  /// @param self reference to `multivector` value
  ///
  /// Forwards `value` unchanged.
  ///
  template <class Self>
    requires (std::is_same_v<multivector, std::remove_cvref_t<Self>>)
  friend constexpr auto to_multivector(Self&& self) noexcept -> Self&&
  {
    return std::forward<Self>(self);
  }

  /// obtain the type after from invoking `to_multivector` on
  /// `std::declval<T>()`
  ///
  template <class T>
  using to_multivector_t = decltype(to_multivector(std::declval<T>()));

  /// addition
  ///
  /// @{

  /// addition of `multivector`-promotable types
  /// @tparam V1, V2 cv-ref qualified `multivector`-promotable types
  /// @param v1, v2 `multivector`-promotable value
  ///
  /// Constructs a multivector containing `blade` elements from `v1` and `v2`.
  /// For each `blade` type `B` in the returned `multivector`, the value is
  /// equal to:
  /// * `get<B>(v1) + get<B>(v2)` if `B` is an element in both `v1` and `v2`;
  ///   otherwise
  /// * `get<B>(v1)` if `B` is an element in `v1`; otherwise
  /// * `get<B>(v2)` if `B` is an element in `v2`
  ///
  /// @note addition of the same `multivector` type, if
  ///   `std::is_same_v<multivector, std::remove_cvref_t<V1>> !=
  ///    std::is_same_v<multivector, std::remove_cvref_t<V2>>` is `true`
  ///  is defined by `derive_vector_space_operations`.
  ///
  template <class V1, class V2>
    requires (std::is_same_v<multivector, std::remove_cvref_t<V1>> !=
              std::is_same_v<multivector, std::remove_cvref_t<V2>>) and
                 requires {
                   to_multivector(std::declval<V1>());
                   to_multivector(std::declval<V2>());
                 } and
                 (not detail::is_defined_v<
                     detail::overload<std::plus<>, V1, V2>>)
  friend constexpr auto operator+(V1&& v1, V2&& v2)
      -> detail::rebind_args_into_t<
          detail::type_concat_t<
              detail::type_list<algebra_type>,
              typename detail::rebind_args_into_t<
                  detail::type_concat_t<
                      typename std::remove_cvref_t<
                          to_multivector_t<V1>>::blade_list_type,
                      typename std::remove_cvref_t<
                          to_multivector_t<V2>>::blade_list_type>,
                  sorted_canonical_blades>::type>,
          multivector>
  {
    using result_blade_list_type = typename detail::rebind_args_into_t<
        detail::type_concat_t<
            typename std::remove_cvref_t<to_multivector_t<V1>>::blade_list_type,
            typename std::remove_cvref_t<
                to_multivector_t<V2>>::blade_list_type>,
        sorted_canonical_blades>::type;

    return []<class... Ts, class V3, class V4>(
               detail::type_list<Ts...>, V3&& v3, V4&& v4) {
      return multivector<A, Ts...>{(
          get_or<Ts>(std::forward<V3>(v3), zero_constant<A>{}) +
          get_or<Ts>(std::forward<V4>(v4), zero_constant<A>{}))...};
    }(result_blade_list_type{},
           to_multivector(std::forward<V1>(v1)),
           to_multivector(std::forward<V2>(v2)));
  }

  /// @}

  /// wedge product
  ///
  /// @{

  /// wedge product of `multivector`-promotable types
  /// @tparam V1, V2 cv-ref qualified `multivector`-promotable types
  /// @param v1, v2 `multivector`-promotable value
  ///
  /// Calculates the wedge product of two multivectors v1 and v2. If v1 and v2
  /// are expressed as a linear combination of blades (b1...) and (b2...), then
  /// the result of `v1 ^ v2` is equivalent to:
  /// ~~~{.cpp}
  /// transform_reduce(
  ///   cartesian_product((b1...), (b2...)),
  ///   zero,
  ///   std::plus<>{},
  ///   wedge)
  /// ~~~
  /// if `(b1...)` and `(b2...)` were valid ranges.
  ///
  template <class V1, class V2>
    requires (std::is_same_v<multivector, std::remove_cvref_t<V1>> or
              std::is_same_v<multivector, std::remove_cvref_t<V2>>) and
             requires {
               to_multivector(std::declval<V1>());
               to_multivector(std::declval<V2>());
             } and
             (not detail::is_defined_v<
                 detail::overload<decltype(wedge), V1, V2>>)
  friend constexpr auto operator^(V1&& v1, V2&& v2)
  {
    const auto& v3 = to_multivector(std::forward<V1>(v1));
    const auto& v4 = to_multivector(std::forward<V2>(v2));

    static constexpr auto N = std::remove_cvref_t<decltype(v3)>::size;
    static constexpr auto M = std::remove_cvref_t<decltype(v4)>::size;

    return []<std::size_t... Is>(
               std::index_sequence<Is...>, const auto& t1, const auto& t2) {
      return ((std::get<Is / M>(t1) ^ std::get<Is % M>(t2)) + ...);
    }(std::make_index_sequence<N * M>{}, v3, v4);
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
template <class B1, class B2, class A = common_algebra_type_t<B1, B2>>
  requires is_blade_v<std::remove_cvref_t<B1>> and
           is_blade_v<std::remove_cvref_t<B2>> and
           (not std::is_same_v<canonical_type_t<B1>, canonical_type_t<B2>>)
// false positive
// NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
constexpr auto operator+(B1&& b1, B2&& b2)
{
  // clang-format off
  return [
    tmp = std::tuple{
      std::forward<B1>(b1).canonical(),
      std::forward<B2>(b2).canonical()
    }  // clang-format on
  ]<template <class...> class list, class... Bs>(list<Bs...>) mutable {
    return multivector{std::get<Bs>(std::move(tmp))...};
  }(sorted_canonical_blades_t<B1, B2>{});
}

/// @}

}  // namespace rigid_geometric_algebra

template <class A, class... Bs, class CharT>
struct std::formatter<::rigid_geometric_algebra::multivector<A, Bs...>, CharT>
    : std::formatter<::rigid_geometric_algebra::algebra_field_t<A>, CharT>
{
  // https://github.com/llvm/llvm-project/issues/66466
  template <class O>
  constexpr auto
  format(const ::rigid_geometric_algebra::multivector<A, Bs...>& v,
         std::basic_format_context<O, CharT>& ctx) const
  {
    auto out = ctx.out();

    if constexpr (sizeof...(Bs) == 0) {
      return std::format_to(
          out, "{}", ::rigid_geometric_algebra::algebra_field_t<A>{});
    } else {
      out = std::format_to(out, "{}", std::get<0>(v));

      [&out, &v]<std::size_t... Is>(std::index_sequence<Is...>) {
        std::ignore =
            ((out = std::format_to(out, " + {}", std::get<Is + 1>(v)), true) and
             ...);
      }(std::make_index_sequence<sizeof...(Bs) - 1>{});

      return out;
    }
  }
};
