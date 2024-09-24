#pragma once

#include "detail/even.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/blade_complement_type.hpp"
#include "rigid_geometric_algebra/detail/swaps_to_sorted_dimensions.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/is_multivector.hpp"
#include "rigid_geometric_algebra/multivector.hpp"
#include "rigid_geometric_algebra/multivector_type_from_blade_list.hpp"
#include "rigid_geometric_algebra/sorted_canonical_blades.hpp"
#include "rigid_geometric_algebra/zero_constant.hpp"

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {

/// tags for specifying a direction
///
/// @{

struct left_t
{
  explicit left_t() = default;
};
inline constexpr auto left = left_t{};

struct right_t
{
  explicit right_t() = default;
};
inline constexpr auto right = right_t{};

/// @}

namespace detail {

class complement_fn
{
  template <class B1, class B2>
  struct num_swaps
  {};

  template <class A, std::size_t... Is, std::size_t... Js>
  struct num_swaps<blade<A, Is...>, blade<A, Js...>>
      : std::integral_constant<
            std::size_t,
            detail::swaps_to_sorted_dimensions(Is..., Js...)>
  {};

  template <std::size_t N, class B, class T = std::remove_cvref_t<B>>
  static constexpr auto impl2(std::integral_constant<std::size_t, N>, B&& b)
  {
    using blade_type = std::remove_cvref_t<B>;

    using maybe_negate =
        std::conditional_t<detail::even(N), std::identity, std::negate<>>;

    return blade_complement_type_t<blade_type>{
        maybe_negate{}(std::forward<B>(b).coefficient)};
  }

  template <class Dir, class B>
  static constexpr auto impl(Dir, B&& b)
  {
    static_assert(std::is_same_v<left_t, Dir> or std::is_same_v<right_t, Dir>);
    using B1 = std::remove_cvref_t<B>;
    using B2 = blade_complement_type_t<B1>;

    if constexpr (std::is_same_v<left_t, Dir>) {
      return impl2(num_swaps<B2, B1>{}, std::forward<B>(b));
    } else {
      return impl2(num_swaps<B1, B2>{}, std::forward<B>(b));
    }
  }

  template <
      class Dir,
      template <class...>
      class list,
      class... Bs,
      class V,
      class A = algebra_type_t<std::remove_cvref_t<V>>>
  static constexpr auto mv_impl2(Dir, list<Bs...>, V&& v)
      -> multivector_type_from_blade_list_t<
          sorted_canonical_blades_t<blade_complement_type_t<Bs>...>>
  {
    return (operator()(Dir{}, std::get<Bs>(std::forward<V>(v))) + ... +
            zero_constant<A>{});
  }

  template <class Dir, class V>
  static constexpr auto mv_impl(Dir, V&& v)
      -> decltype(mv_impl2(
          Dir{},
          typename std::remove_cvref_t<V>::blade_list_type{},
          std::forward<V>(v)))
  {
    return mv_impl2(
        Dir{},
        typename std::remove_cvref_t<V>::blade_list_type{},
        std::forward<V>(v));
  }

public:
  // https://github.com/llvm/llvm-project/issues/109738
  // don't use static call operator to avoid segfault with clang-tidy
  //
  // @{

  template <class A>
  constexpr auto operator()(left_t, zero_constant<A>) const -> zero_constant<A>
  {
    return {};
  }
  template <class A>
  constexpr auto operator()(right_t, zero_constant<A>) const -> zero_constant<A>
  {
    return {};
  }

  // @}

  template <class B>
    requires is_blade_v<std::remove_cvref_t<B>>
  static constexpr auto
  operator()(left_t, B&& b) -> blade_complement_type_t<std::remove_cvref_t<B>>
  {
    return impl(left_t{}, std::forward<B>(b));
  }
  template <class B>
    requires is_blade_v<std::remove_cvref_t<B>>
  static constexpr auto
  operator()(right_t, B&& b) -> blade_complement_type_t<std::remove_cvref_t<B>>
  {
    return impl(right_t{}, std::forward<B>(b));
  }
  template <class V>
    requires is_multivector_v<std::remove_cvref_t<V>>
  static constexpr auto
  operator()(left_t, V&& v) -> decltype(mv_impl(left_t{}, std::forward<V>(v)))
  {
    return mv_impl(left_t{}, std::forward<V>(v));
  }
  template <class V>
    requires is_multivector_v<std::remove_cvref_t<V>>
  static constexpr auto
  operator()(right_t, V&& v) -> decltype(mv_impl(right_t{}, std::forward<V>(v)))
  {
    return mv_impl(right_t{}, std::forward<V>(v));
  }
};

}  // namespace detail

/// obtains the left or right complement of a blade
/// @tparam D direction type
/// @tparam B blade type
/// @param d direction
/// @param b blade
///
/// Returns the left or right complement of `b`, where the direction is
/// specified by `d`.
///
/// The left complement is defined as:
/// ```
/// left_complement(b) ^ b == s * s * A::antiscalar{1}
/// ```
/// where `A` is `algebra_type_t<B>`.
/// where `A` is `algebra_type_t<B>` and `s` is the cofficient of `b`.
///
/// @note Requires:
/// * `std::is_same_v<D, left_t> or std::is_same_v<D, right_t>` is `true`
/// @see eq. 2.13
/// @see eq. 2.19
/// @see eq. 2.20
/// @see eq. 2.24
///
inline constexpr auto complement = detail::complement_fn{};

/// obtains the left complement of a blade
/// @tparam B blade type
/// @param b blade
///
/// Returns the left complement of `b`.
///
/// The left complement is defined as:
/// ```
/// left_complement(b) ^ b == s * s * A::antiscalar{1}
/// ```
/// where `A` is `algebra_type_t<B>` and `s` is the cofficient of `b`.
///
/// @see eq. 2.13
/// @see eq. 2.20
/// @see eq. 2.24
///
inline constexpr auto left_complement = std::bind_front(complement, left);

/// obtains the right complement of a blade
/// @tparam B blade type
/// @param b blade
///
/// Returns the right complement of `b`.
///
/// The right complement is defined as:
/// ```
/// b ^ right_complement(b) == s * s * A::antiscalar{1}
/// ```
/// where `A` is `algebra_type_t<B>` and `s` is the cofficient of `b`.
///
/// @see eq. 2.13
/// @see eq. 2.19
/// @see eq. 2.24
///
inline constexpr auto right_complement = std::bind_front(complement, right);

}  // namespace rigid_geometric_algebra
