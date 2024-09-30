#pragma once

#include "detail/even.hpp"
#include "rigid_geometric_algebra/blade_complement_type.hpp"
#include "rigid_geometric_algebra/detail/concat_ranges.hpp"
#include "rigid_geometric_algebra/detail/counted_sort.hpp"
#include "rigid_geometric_algebra/detail/linear_operator.hpp"
#include "rigid_geometric_algebra/detail/negate_if_odd.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"

#include <cstddef>
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
public:
  template <class Dir, detail::blade B>
  static consteval auto operator()(Dir, std::type_identity<B>) -> bool
  {
    static constexpr auto odd_number_of_swaps =
        [](const auto& r1, const auto& r2) {
          return not detail::even(
              detail::counted_sort(detail::concat_ranges(r1, r2)));
        };

    if constexpr (std::is_same_v<Dir, left_t>) {
      return odd_number_of_swaps(
          blade_complement_type_t<B>::dimensions,
          std::remove_cvref_t<B>::dimensions);
    } else if constexpr (std::is_same_v<Dir, right_t>) {
      return odd_number_of_swaps(
          std::remove_cvref_t<B>::dimensions,
          blade_complement_type_t<B>::dimensions);
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
class complement_blade_fn
{
  static_assert(std::is_same_v<Dir, left_t> or std::is_same_v<Dir, right_t>);

public:
  template <detail::blade B>
  static constexpr auto operator()(B&& b) -> blade_complement_type_t<B>
  {
    static constexpr auto sign =
        std::size_t(detail::blade_complement_negates<Dir, B>);

    return blade_complement_type_t<B>{
        detail::negate_if_odd<sign>{}(std::forward<B>(b).coefficient)};
  }
};

template <class Dir>
using complement_fn = detail::linear_operator<detail::complement_blade_fn<Dir>>;

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
