#pragma once

#include "detail/even.hpp"
#include "rigid_geometric_algebra/blade_complement_type.hpp"
#include "rigid_geometric_algebra/detail/derive_multivector_overload.hpp"
#include "rigid_geometric_algebra/detail/derive_zero_constant_overload.hpp"
#include "rigid_geometric_algebra/detail/swaps_to_sorted_dimensions.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"

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

/// true if the specified complement negates the blade coefficient, otherwise
/// false
///
/// @{

class blade_complement_negates_fn
{
  template <class B1, class B2>
  struct num_swaps
  {};

  template <
      template <class, std::size_t...> class blade_,
      class A,
      std::size_t... Is,
      std::size_t... Js>
  struct num_swaps<blade_<A, Is...>, blade_<A, Js...>>
      : std::integral_constant<
            std::size_t,
            detail::swaps_to_sorted_dimensions(Is..., Js...)>
  {};

public:
  template <class Dir, detail::blade B>
  static consteval auto operator()(Dir, std::type_identity<B>) -> bool
  {
    if constexpr (std::is_same_v<Dir, left_t>) {
      return detail::even(num_swaps<blade_complement_type_t<B>, B>{});
    } else if constexpr (std::is_same_v<Dir, right_t>) {
      return detail::even(num_swaps<B, blade_complement_type_t<B>>{});
    } else {
      static_assert(false, "invalid direction type");
    }
  }
};

template <class Dir, class B>
inline constexpr auto blade_complement_negates = blade_complement_negates_fn{}(
    Dir{}, std::type_identity<std::remove_cvref_t<B>>{});

/// @}

template <class Dir>
class complement_fn
    : detail::derive_zero_constant_overload<complement_fn<Dir>>,
      detail::derive_multivector_overload<complement_fn<Dir>>
{
  static_assert(std::is_same_v<Dir, left_t> or std::is_same_v<Dir, right_t>);

public:
  using detail::derive_zero_constant_overload<complement_fn<Dir>>::operator();
  using detail::derive_multivector_overload<complement_fn<Dir>>::operator();

  template <class B>
    requires is_blade_v<std::remove_cvref_t<B>>
  static constexpr auto
  operator()(B&& b) -> blade_complement_type_t<std::remove_cvref_t<B>>
  {
    using maybe_negate = std::conditional_t<
        detail::blade_complement_negates<Dir, B>,
        std::identity,
        std::negate<>>;

    return blade_complement_type_t<B>{
        maybe_negate{}(std::forward<B>(b).coefficient)};
  }
};

}  // namespace detail

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
inline constexpr auto left_complement = detail::complement_fn<left_t>{};

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
inline constexpr auto right_complement = detail::complement_fn<right_t>{};

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
inline constexpr struct
{
  template <class Dir, class T>
  static constexpr auto operator()(Dir, T&& value)
      -> decltype(detail::complement_fn<Dir>{}(std::forward<T>(value)))
  {
    return detail::complement_fn<Dir>{}(std::forward<T>(value));
  }
} complement{};

}  // namespace rigid_geometric_algebra
