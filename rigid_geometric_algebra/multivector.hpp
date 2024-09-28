#pragma once

#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/copy_ref_qual.hpp"
#include "rigid_geometric_algebra/detail/decays_to.hpp"
#include "rigid_geometric_algebra/detail/derive_subtraction.hpp"
#include "rigid_geometric_algebra/detail/derive_vector_space_operations.hpp"
#include "rigid_geometric_algebra/detail/is_defined.hpp"
#include "rigid_geometric_algebra/detail/multivector_promotable.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"
#include "rigid_geometric_algebra/detail/size_checked_subrange.hpp"
#include "rigid_geometric_algebra/detail/type_concat.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"
#include "rigid_geometric_algebra/get.hpp"
#include "rigid_geometric_algebra/get_or.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"
#include "rigid_geometric_algebra/to_multivector.hpp"
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
namespace detail {
// forward declaration
// this function object is invoked by hidden friend operator^
class wedge_fn;
}  // namespace detail

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
    : std::tuple<Bs...>,
      detail::derive_vector_space_operations<
          multivector<A, Bs...>,
          detail::get_fn<Bs>...>,
      detail::derive_subtraction<multivector<A, Bs...>>
{
  using base_type = std::tuple<Bs...>;

public:
  using type = multivector;

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
  using base_type::base_type;

  /// blade list
  ///
  using blade_list_type = detail::type_list<Bs...>;

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

  /// access the underlying tuple
  ///
  template <class Self>
  constexpr auto
  as_tuple(this Self&& self) -> detail::copy_ref_qual_t<Self, std::tuple<Bs...>>
  {
    return static_cast<detail::copy_ref_qual_t<Self, std::tuple<Bs...>>>(
        std::forward<Self>(self));
  }

  /// access blade element
  /// @tparam B blade type to access
  ///
  /// @{

  template <class B, class Self>
    requires contains<B>
  constexpr auto get(this Self&& self) -> detail::copy_ref_qual_t<Self&&, B>
  {
    return std::get<B>(std::forward<Self>(self).as_tuple());
  }

  template <class B, detail::decays_to<multivector> Self>
  friend constexpr auto
  get(Self&& self) -> decltype(std::forward<Self>(self).template get<B>())
  {
    return std::forward<Self>(self).template get<B>();
  }

  /// @}

  /// access blade element
  /// @tparam I blade type to access
  ///
  /// @{

  template <std::size_t I, class Self>
    requires (I < size)
  constexpr auto get(this Self&& self)
      -> decltype(std::forward<Self>(self)
                      .template get<std::tuple_element_t<I, multivector>>())
  {
    return std::forward<Self>(self)
        .template get<std::tuple_element_t<I, multivector>>();
  }

  template <std::size_t I, detail::decays_to<multivector> Self>
  friend constexpr auto
  get(Self&& self) -> decltype(std::forward<Self>(self).template get<I>())
  {
    return std::forward<Self>(self).template get<I>();
  }

  /// @}

  /// promote value to a `multivector`
  /// @tparam Self `multivector` type
  /// @param self reference to `multivector` value
  ///
  /// Forwards `value` unchanged.
  ///
  template <detail::decays_to<multivector> Self>
  friend constexpr auto to_multivector(Self&& self) noexcept -> Self&&
  {
    return std::forward<Self>(self);
  }

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
  template <
      detail::multivector_promotable V1,
      detail::multivector_promotable V2>
    requires (detail::decays_to<V1, multivector> !=
              detail::decays_to<V2, multivector>) and
             (not detail::is_defined_v<detail::overload<std::plus<>, V1, V2>>)
  friend constexpr auto
  operator+(V1&& v1, V2&& v2) -> typename detail::type_concat_t<
      typename std::remove_cvref_t<to_multivector_t<V1>>::blade_list_type,
      typename std::remove_cvref_t<to_multivector_t<V2>>::blade_list_type>::
      template insert_into_t<sorted_canonical_blades<>>::template insert_into_t<
          multivector<algebra_type>>
  {
    using result_blade_list_type = typename detail::type_concat_t<
        typename std::remove_cvref_t<to_multivector_t<V1>>::blade_list_type,
        typename std::remove_cvref_t<to_multivector_t<V2>>::blade_list_type>::
        template insert_into_t<sorted_canonical_blades<>>;

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
  template <class F = detail::wedge_fn>
  friend constexpr auto operator^(const multivector& lhs, const auto& rhs)
      -> decltype(std::declval<F>()(lhs, rhs))
  {
    return F{}(lhs, rhs);
  }

  /// @}

  /// equality comparison
  ///
  /// @{

  friend auto
  operator==(const multivector&, const multivector&) -> bool = default;

  template <class... B2s>
    requires (not std::is_same_v<multivector, multivector<A, B2s...>>)
  friend auto operator==(const multivector&, const multivector<A, B2s...>&)
      -> bool = delete;

  /// @}
};

/// deduction guide
///
template <class B0, class... Bs>
multivector(const B0&, const Bs&...)
    -> multivector<algebra_type_t<B0>, B0, Bs...>;

/// promote `blade` to a `multivector`
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
    detail::blade B1,
    detail::blade B2,
    class A = common_algebra_type_t<B1, B2>>
  requires (not std::is_same_v<canonical_type_t<B1>, canonical_type_t<B2>>)
constexpr auto operator+(B1&& b1, B2&& b2) -> sorted_canonical_blades_t<
    canonical_type_t<B1>,
    canonical_type_t<B2>>::template insert_into_t<multivector<A>>
{
  if constexpr (
      blade_ordering<A>{std::remove_cvref_t<B1>::dimension_mask} <
      blade_ordering<A>{std::remove_cvref_t<B2>::dimension_mask}) {
    return {std::forward<B1>(b1).canonical(), std::forward<B2>(b2).canonical()};
  } else {
    return {std::forward<B2>(b2).canonical(), std::forward<B1>(b1).canonical()};
  }
}

/// @}

}  // namespace rigid_geometric_algebra

template <class A, class... Bs>
struct std::tuple_size<::rigid_geometric_algebra::multivector<A, Bs...>>
    : std::integral_constant<std::size_t, sizeof...(Bs)>
{};

template <std::size_t I, class A, class... Bs>
struct std::tuple_element<I, ::rigid_geometric_algebra::multivector<A, Bs...>>
{
  using type = std::tuple_element_t<I, std::tuple<Bs...>>;
};

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
      out = std::format_to(out, "{}", v.template get<0>());

      [&out, &v]<std::size_t... Is>(std::index_sequence<Is...>) {
        std::ignore =
            ((out = std::format_to(out, " + {}", v.template get<Is + 1>()),
              true) and
             ...);
      }(std::make_index_sequence<sizeof...(Bs) - 1>{});

      return out;
    }
  }
};
