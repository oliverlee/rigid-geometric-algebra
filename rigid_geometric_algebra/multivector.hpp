#pragma once

#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/blade_dimensions.hpp"
#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/blade_type_from.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/copy_ref_qual.hpp"
#include "rigid_geometric_algebra/detail/decays_to.hpp"
#include "rigid_geometric_algebra/detail/derive_subtraction.hpp"
#include "rigid_geometric_algebra/detail/derive_vector_space_operations.hpp"
#include "rigid_geometric_algebra/detail/multivector_promotable.hpp"
#include "rigid_geometric_algebra/detail/multivector_sum.hpp"
#include "rigid_geometric_algebra/detail/rebind_args_into.hpp"
#include "rigid_geometric_algebra/detail/size_checked_subrange.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"
#include "rigid_geometric_algebra/get.hpp"
#include "rigid_geometric_algebra/get_or.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/is_canonical_blade_order.hpp"
#include "rigid_geometric_algebra/to_multivector.hpp"
#include "rigid_geometric_algebra/wedge.hpp"

#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <ranges>
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
template <class A, blade_dimensions... D>
  requires is_algebra_v<A>
class multivector
    : std::tuple<blade_type_from_dimensions_t<A, D>...>,
      detail::derive_vector_space_operations<
          multivector<A, D...>,
          detail::get_fn<blade_type_from_dimensions_t<A, D>>...>,
      detail::derive_subtraction<multivector<A, D...>>
{
  static_assert(
      is_canonical_blade_order<blade_type_from_dimensions_t<A, D>...>());

  using base_type = std::tuple<blade_type_from_dimensions_t<A, D>...>;

  struct blade_set : blade_type_from_dimensions_t<A, D>...
  {};

  // can't use immediately invoked lambda, likely due to
  // https://github.com/llvm/llvm-project/issues/93327
  // https://www.reddit.com/r/cpp_questions/comments/1961soo/immediatelyinvoked_lambda_in_concept_definition/
  template <auto... D2>
  static constexpr auto dimensions_subset() -> bool
  {
    constexpr auto r1 = std::array<blade_ordering<A>, sizeof...(D)>{
        blade_type_from_dimensions_t<A, D>::ordering...};
    constexpr auto r2 = std::array<blade_ordering<A>, sizeof...(D2)>{
        blade_type_from_dimensions_t<A, D2>::ordering...};

    return std::includes(r1.begin(), r1.end(), r2.begin(), r2.end());
  }

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
      std::integral_constant<std::size_t, sizeof...(D)>{};

  /// inherited constructors from std::tuple
  ///
  using base_type::base_type;

  /// blade list
  ///
  using blade_list_type =
      detail::rebind_args_into_t<base_type, detail::type_list>;

  /// non-narrowing construction from a different multivector
  ///
  template <blade_dimensions... D2>
    requires (dimensions_subset<D2...>())
  constexpr explicit multivector(const multivector<algebra_type, D2...>& other)
      : base_type{[&other]<class... Bs>(detail::type_list<Bs...>) {
          return base_type{get_or<Bs>(other, Bs{})...};
        }(blade_list_type{})}
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
      : base_type{
            []<std::size_t... Is>(std::index_sequence<Is...>, auto values) {
              return base_type{values[Is]...};
            }(std::make_index_sequence<size>{},
              detail::size_checked_subrange<size>(il))}
  {}

  /// checks if a blade is contained in the multivector
  ///
  template <class B>
  static constexpr auto contains = std::is_base_of_v<B, blade_set>;

  /// access the underlying tuple
  ///
  template <class Self>
  constexpr auto
  as_tuple(this Self&& self) -> detail::copy_ref_qual_t<Self, base_type>
  {
    return static_cast<detail::copy_ref_qual_t<Self, base_type>>(
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
      -> decltype(std::get<I>(std::forward<Self>(self).as_tuple()))
  {
    return std::get<I>(std::forward<Self>(self).as_tuple());
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
      detail::decays_to<multivector> V1,
      detail::multivector_promotable V2>
    requires has_common_algebra_type_v<V1, V2> and
             (not detail::decays_to<V2, multivector>)
  friend constexpr auto
  operator+(V1&& v1, V2&& v2) -> decltype(detail::multivector_sum(
      std::forward<V1>(v1), to_multivector(std::forward<V2>(v2))))
  {
    return detail::multivector_sum(
        std::forward<V1>(v1), to_multivector(std::forward<V2>(v2)));
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
  template <class T>
  friend constexpr auto
  operator^(const multivector& lhs, const T& rhs) -> decltype(wedge(lhs, rhs))
  {
    return wedge(lhs, rhs);
  }

  /// @}

  /// equality comparison
  ///
  /// @{

  friend auto
  operator==(const multivector&, const multivector&) -> bool = default;

  template <auto... D2>
    requires (not std::is_same_v<multivector, multivector<A, D2...>>)
  friend auto
  operator==(const multivector&, const multivector<A, D2...>&) -> bool = delete;

  /// @}
};

/// deduction guide
///
template <detail::blade B0, detail::blade... Bs>
multivector(const B0&, const Bs&...)
    -> multivector<algebra_type_t<B0>, B0::dimensions, Bs::dimensions...>;

}  // namespace rigid_geometric_algebra

template <class A, auto... D>
struct std::tuple_size<::rigid_geometric_algebra::multivector<A, D...>>
    : std::integral_constant<std::size_t, sizeof...(D)>
{};

template <std::size_t I, class A, auto... D>
struct std::tuple_element<I, ::rigid_geometric_algebra::multivector<A, D...>>
{
  using type = std::remove_cvref_t<
      decltype(std::declval<::rigid_geometric_algebra::multivector<A, D...>>()
                   .template get<I>())>;
};

template <class A, auto... D, class CharT>
struct std::formatter<::rigid_geometric_algebra::multivector<A, D...>, CharT>
    : std::formatter<::rigid_geometric_algebra::algebra_field_t<A>, CharT>
{
  // https://github.com/llvm/llvm-project/issues/66466
  template <class O>
  constexpr auto
  format(const ::rigid_geometric_algebra::multivector<A, D...>& v,
         std::basic_format_context<O, CharT>& ctx) const
  {
    auto out = ctx.out();

    if constexpr (sizeof...(D) == 0) {
      return std::format_to(
          out, "{}", ::rigid_geometric_algebra::algebra_field_t<A>{});
    } else {
      out = std::format_to(out, "{}", v.template get<0>());

      [&out, &v]<std::size_t... Is>(std::index_sequence<Is...>) {
        std::ignore =
            ((out = std::format_to(out, " + {}", v.template get<Is + 1>()),
              true) and
             ...);
      }(std::make_index_sequence<sizeof...(D) - 1>{});

      return out;
    }
  }
};
