#pragma once

#include "rigid_geometric_algebra/algebra_dimension.hpp"
#include "rigid_geometric_algebra/algebra_field.hpp"
#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/blade_sum.hpp"
#include "rigid_geometric_algebra/canonical_type.hpp"
#include "rigid_geometric_algebra/detail/are_dimensions_unique.hpp"
#include "rigid_geometric_algebra/detail/counted_sort.hpp"
#include "rigid_geometric_algebra/detail/decays_to.hpp"
#include "rigid_geometric_algebra/detail/derive_subtraction.hpp"
#include "rigid_geometric_algebra/detail/derive_vector_space_operations.hpp"
#include "rigid_geometric_algebra/detail/indices_array.hpp"
#include "rigid_geometric_algebra/detail/multivector_sum.hpp"
#include "rigid_geometric_algebra/detail/negate_if_odd.hpp"
#include "rigid_geometric_algebra/detail/size_checked_subrange.hpp"
#include "rigid_geometric_algebra/detail/structural_bitset.hpp"
#include "rigid_geometric_algebra/glz_fwd.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"
#include "rigid_geometric_algebra/to_multivector.hpp"
#include "rigid_geometric_algebra/wedge.hpp"

#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <initializer_list>
#include <tuple>
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
/// Every blade has a grade, where the grade is the number of factors, equal to
/// `sizeof(Is...)`. The grade of the blade cannot exceed the algebra's
/// dimension.
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
               (detail::are_dimensions_unique(Is...))
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
  static constexpr auto grade =
      std::integral_constant<std::size_t, sizeof...(Is)>{};

  /// bitset type specifying the dimension mask
  ///
  using dimension_mask_type = detail::structural_bitset<algebra_dimension_v<A>>;

  /// factors that are present in this blade
  ///
  static constexpr auto dimension_mask =
      (dimension_mask_type{}.set(Is) | ... | dimension_mask_type{});

  /// factors
  ///
  static constexpr auto dimensions = std::array<std::size_t, grade>{Is...};

  /// ordering
  ///
  static constexpr auto ordering = blade_ordering<algebra_type>{dimension_mask};

  /// blade scalar type
  ///
  using value_type = algebra_field_t<A>;

  /// blade type with indices in canonical form
  ///
  using canonical_type =
      decltype([]<std::size_t... Js>(std::index_sequence<Js...>)
                   -> blade<A, detail::indices_array<dimension_mask>[Js]...> {
        return {};
      }(std::make_index_sequence<sizeof...(Is)>{}));

  /// blade scalar coefficient
  ///
  value_type coefficient{};

  /// default constructor
  ///
  /// constructs a blade with a zero coefficient
  ///
  blade() = default;

  /// initializer list constructor
  /// @param il initializer list of values
  ///
  /// @note allows conversions to `value_type` is floating point and if the
  ///   source can be stored exactly
  /// @see
  /// https://en.cppreference.com/w/cpp/language/list_initialization#Narrowing_conversions
  ///
  constexpr blade(std::initializer_list<value_type> il)
    requires std::floating_point<value_type>
      : coefficient{detail::size_checked_subrange<1>(il)[0]}
  {}

  /// converting constructor
  /// @tparam T type `value_type` is constructible from
  /// @tparam value coefficient value
  ///
  /// constructs a blade with coefficient specified by value
  ///
  template <class T>
    requires std::constructible_from<value_type, T>
  constexpr explicit(grade != 0) blade(T&& value)
      : coefficient{std::forward<T>(value)}
  {}

  /// obtain the blade with indices in canonical form
  ///
  /// Returns same blade expressed in canonical form - i.e. with indices in
  /// increasing order. The blade coefficient is negated if sorting the indices
  /// requires an odd number of swaps.
  ///
  template <class Self>
  constexpr auto canonical(this Self&& self) -> canonical_type
  {
    return canonical_type{detail::negate_if_odd<detail::counted_sort(
        auto{dimensions})>{}(std::forward<Self>(self).coefficient)};
  }

  /// addition
  ///
  /// @{

  /// add different blades types with the same canonical type
  ///
  template <detail::decays_to<blade> B1, detail::blade B2>
    requires (not detail::decays_to<B2, blade>) and
             std::is_same_v<canonical_type, canonical_type_t<B2>> and
             has_common_algebra_type_v<B1, B2>
  friend constexpr auto operator+(B1&& b1, B2&& b2) -> canonical_type
  {
    return std::forward<B1>(b1).canonical() + std::forward<B2>(b2).canonical();
  }

  /// add different blades types with different canonical type
  ///
  template <detail::decays_to<blade> B1, detail::blade B2>
    requires (not detail::decays_to<B2, blade>) and
             (not std::is_same_v<canonical_type, canonical_type_t<B2>>) and
             has_common_algebra_type_v<B1, B2>
  friend constexpr auto operator+(B1&& b1, B2&& b2)
      -> decltype(blade_sum(std::forward<B1>(b1), std::forward<B2>(b2)))
  {
    return blade_sum(std::forward<B1>(b1), std::forward<B2>(b2));
  }

  /// add a blade with a multivector
  ///
  template <detail::decays_to<blade> B1, detail::multivector V2>
    requires has_common_algebra_type_v<B1, V2>
  friend constexpr auto
  operator+(B1&& b1, V2&& v2) -> decltype(detail::multivector_sum(
      to_multivector(std::forward<B1>(b1)), std::forward<V2>(v2)))
  {
    return detail::multivector_sum(
        to_multivector(std::forward<B1>(b1)), std::forward<V2>(v2));
  }

  /// @}

  /// wedge product
  ///
  /// @{

  template <class T>
  friend constexpr auto
  operator^(const blade& lhs, const T& rhs) -> decltype(wedge(lhs, rhs))
  {
    return wedge(lhs, rhs);
  }

  /// @}

  /// equality comparison
  ///
  /// @{

  friend auto operator==(const blade&, const blade&) -> bool = default;

  template <std::size_t... Js>
    requires (std::is_same_v<canonical_type, canonical_type_t<blade<A, Js...>>>)
  friend constexpr auto
  operator==(const blade& lhs, const blade<A, Js...>& rhs) -> bool
  {
    return lhs.canonical() == rhs.canonical();
  }

  /// @}
};

}  // namespace rigid_geometric_algebra

template <class A, std::size_t... Is, class CharT>
struct std::formatter<::rigid_geometric_algebra::blade<A, Is...>, CharT>
    : std::formatter<::rigid_geometric_algebra::algebra_field_t<A>, CharT>
{
  static_assert(::rigid_geometric_algebra::algebra_dimension_v<A> <= 10);

  // https://github.com/llvm/llvm-project/issues/66466
  template <class Context>
  constexpr auto format(
      const ::rigid_geometric_algebra::blade<A, Is...>& b, Context& ctx) const
  {
    auto out = std::formatter<::rigid_geometric_algebra::algebra_field_t<A>>::
        format(b.coefficient, ctx);

    if (sizeof...(Is) == 0) {
      return out;
    }

    // https://en.wikipedia.org/wiki/Unicode_subscripts_and_superscripts#Superscripts_and_subscripts_block
    static constexpr auto subscripts = std::array{
        "₀",
        "₁",
        "₂",
        "₃",
        "₄",
        "₅",
        "₆",
        "₇",
        "₈",
        "₉",
    };

    out = std::format_to(out, "e");
    std::ignore = ((out = std::format_to(out, subscripts[Is]), true) and ...);
    return out;
  }
};

template <class A, std::size_t... Is>
struct ::glz::meta<::rigid_geometric_algebra::blade<A, Is...>>
{
  using T = ::rigid_geometric_algebra::blade<A, Is...>;
  static constexpr auto value = &T::coefficient;
};
