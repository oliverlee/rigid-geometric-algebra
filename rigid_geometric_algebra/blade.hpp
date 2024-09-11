#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/detail/derive_subtraction.hpp"
#include "rigid_geometric_algebra/detail/derive_vector_space_operations.hpp"
#include "rigid_geometric_algebra/detail/sorted_dimensions.hpp"
#include "rigid_geometric_algebra/detail/swaps_to_sorted_dimensions.hpp"
#include "rigid_geometric_algebra/detail/unique_dimensions.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {

namespace detail {
struct get_coefficient
{
  template <class Self>
  constexpr static auto
  operator()(Self&& self) -> decltype(std::forward<Self>(self).coefficient)
  {
    // false positive, no error with member call operator
    // NOLINTNEXTLINE(clang-analyzer-core.uninitialized.UndefReturn)
    return std::forward<Self>(self).coefficient;
  }
};
}  // namespace detail

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
/// `blade<A, 1>` and `blade<A, 2>`. A grade 2 blade is also called a
/// bivector.
///
/// A blade cannot contain repeated index-vector values. The index-vector
/// values must be less than the algebra's projected dimension. This differs
/// from the standard Lengyel notation which is 1-based and not 0-based; 0
/// corresponds to the projected dimension.
///
/// @see https://terathon.com/foundations_pga_lengyel.pdf
///
template <class A, std::size_t... Is>
  requires ((Is < algebra_dimension_v<A>) and ...) and
               (detail::unique_dimensions(Is...))
class blade
    : detail::derive_vector_space_operations<
          blade<A, Is...>,
          detail::get_coefficient>,
      detail::derive_subtraction<blade<A, Is...>>
{
public:
  /// algebra this blade belongs to
  ///
  using algebra_type = A;

  /// number of factors
  ///
  static constexpr auto grade = sizeof...(Is);

  /// blade scalar type
  ///
  using value_type = algebra_field_t<A>;

  /// blade type with indices in canonical form
  ///
  using canonical_type =
      decltype([]<std::size_t... Js>(
                   std::index_sequence<Js...>) -> blade<A, Js...> {
        return {};
      }(detail::sorted_dimensions<Is...>{}));

  /// blade scalar coefficient
  ///
  value_type coefficient{};

  /// default constructor
  ///
  /// constructs a blade with a zero coefficient
  ///
  blade() = default;

  /// coefficient constructor
  /// @param value coefficient value
  ///
  /// constructs a blade with coefficient specified by value
  ///
  constexpr blade(value_type value) : coefficient{std::move(value)} {}

  /// obtain the blade with indices in canonical form
  ///
  /// Returns same blade expressed in canonical form - i.e. with indices in
  /// increasing order. The blade coefficient is negated if sorting the indices
  /// requires an odd number of swaps.
  ///
  template <class Self>
  constexpr auto canonical(this Self&& self) -> canonical_type
  {
    constexpr auto even = [](std::size_t value) { return value % 2UZ == 0UZ; };
    using maybe_negate = std::conditional_t<
        even(detail::swaps_to_sorted_dimensions(Is...)),
        std::identity,
        std::negate<>>;

    return {maybe_negate{}(std::forward<Self>(self).coefficient)};
  }

  /// addition
  ///
  /// @{

  /// add different blades types with the same canonical type
  ///
  template <class T, class B>
    requires std::is_same_v<blade, std::remove_cvref_t<T>> and
                 (not std::is_same_v<std::remove_cvref_t<B>,
                                     std::remove_cvref_t<T>>) and
                 std::is_same_v<typename blade::canonical_type,
                                typename std::remove_cvref_t<B>::canonical_type>
  friend constexpr auto
  operator+(T&& lhs, B&& rhs) -> typename blade::canonical_type
  {
    return std::forward<T>(lhs).canonical() + std::forward<B>(rhs).canonical();
  }

  /// @}

  /// wedge product
  ///
  /// @{

  template <std::size_t... Js>
  friend constexpr auto
  operator^(const blade& lhs, const blade<A, Js...>& rhs) ->
      typename blade<A, Is..., Js...>::canonical_type
  {
    return blade<A, Is..., Js...>{lhs.coefficient * rhs.coefficient}
        .canonical();
  }

  template <std::size_t... Js>
    requires (not detail::unique_dimensions(Is..., Js...))
  friend constexpr auto
  operator^(const blade&, const blade<A, Js...>&) -> zero_constant<A>
  {
    return {};
  }

  /// @}

  /// equality comparison
  ///
  /// @{

  friend auto operator==(const blade&, const blade&) -> bool = default;

  template <std::size_t... Js>
    requires (std::is_same_v<typename blade::canonical_type,
                             typename blade<A, Js...>::canonical_type>)
  friend constexpr auto
  operator==(const blade& lhs, const blade<A, Js...>& rhs) -> bool
  {
    return lhs.canonical() == rhs.canonical();
  }

  /// @}
};

}  // namespace rigid_geometric_algebra
