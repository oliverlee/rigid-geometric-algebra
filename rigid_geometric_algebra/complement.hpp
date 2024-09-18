#pragma once

#include "detail/even.hpp"
#include "rigid_geometric_algebra/algebra_type.hpp"
#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/blade_complement_type.hpp"
#include "rigid_geometric_algebra/detail/contract.hpp"
#include "rigid_geometric_algebra/detail/swaps_to_sorted_dimensions.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"
#include "rigid_geometric_algebra/one.hpp"

#include <cstddef>
#include <functional>
#include <source_location>
#include <type_traits>

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
    using algebra_type = algebra_type_t<blade_type>;

    using maybe_negate =
        std::conditional_t<detail::even(N), std::identity, std::negate<>>;

    return blade_complement_type_t<blade_type>{
        maybe_negate{}(one<algebra_type> / std::forward<B>(b).coefficient)};
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

  template <class B>
  static constexpr auto
  pre(const B& b,
      const std::source_location& loc = std::source_location::current()) -> void
  {
    detail::precondition(
        b != B{},
        detail::contract_violation_handler{"blade coefficient cannot be zero"},
        loc);
  }

public:
  template <class B>
    requires is_blade_v<std::remove_cvref_t<B>>
  static constexpr auto
  operator()(left_t, B&& b) -> blade_complement_type_t<std::remove_cvref_t<B>>
  {
    pre(b);
    return impl(left_t{}, std::forward<B>(b));
  }
  template <class B>
    requires is_blade_v<std::remove_cvref_t<B>>
  static constexpr auto
  operator()(right_t, B&& b) -> blade_complement_type_t<std::remove_cvref_t<B>>
  {
    pre(b);
    return impl(right_t{}, std::forward<B>(b));
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
/// left_complement(b) ^ b == A::antiscalar{1}
/// ```
/// where `A` is `algebra_type_t<B>`.
///
/// @pre `b.coefficient == 0` is `false`
/// @note Requires:
/// * `std::is_same_v<D, left_t> or std::is_same_v<D, right_t>` is `true`
/// @see eq. 2.13
/// @see eq. 2.19
/// @see eq. 2.20
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
/// left_complement(b) ^ b == A::antiscalar{1}
/// ```
/// where `A` is `algebra_type_t<B>`.
///
/// @pre `b.coefficient == 0` is `false`
/// @see eq. 2.13
/// @see eq. 2.20
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
/// b ^ right_complement(b) == A::antiscalar{1}
/// ```
/// where `A` is `algebra_type_t<B>`.
///
/// @pre `b.coefficient == 0` is `false`
/// @see eq. 2.13
/// @see eq. 2.19
///
inline constexpr auto right_complement = std::bind_front(complement, right);

}  // namespace rigid_geometric_algebra
