#pragma once

#include "rigid_geometric_algebra/common_algebra_type.hpp"

#include <type_traits>

namespace rigid_geometric_algebra {

/// compile time zero constant
/// @tparam A specialization of `algebra`
///
/// Represents a value known to be zero at compile-time (e.g. the result of
/// v ^ v for a vector v).
///
template <class A>
struct zero_constant
{
  /// algebra this element belongs to
  ///
  using algebra_type = A;

  /// equality comparison
  ///
  friend auto operator==(zero_constant, zero_constant) -> bool = default;

  /// wedge product
  /// @tparam T1, T2 algebraic types, one of which is `zero_constant`
  /// @note Requires:
  /// * T1 and T2 have the same algebra type
  /// * T1 is `zero_constant` xor T2 is `zero_constant`
  /// @returns `zero_constant` value
  ///
  template <class T1, class T2>
    requires has_common_algebra_type_v<T1, T2> and
                 (std::is_same_v<zero_constant, T1> or
                  std::is_same_v<zero_constant, T2>)
  friend constexpr auto
  operator^(const T1&, const T2&) noexcept -> zero_constant
  {
    return {};
  }

  /// negation
  /// @returns `zero_constant` value
  ///
  friend constexpr auto operator-(zero_constant) noexcept -> zero_constant
  {
    return {};
  }

  /// addition
  /// @tparam T1, T2 algebraic types, exactly one of which is `zero_constant`
  /// @param lhs, rhs arguments
  /// @note Requires:
  /// * T1 and T2 have the same algebra type
  /// * T1 is `zero_constant` xor T2 is `zero_constant`
  /// @returns the non-zero value
  ///
  template <
      class T1,
      class T2,
      class D1 = std::remove_cvref_t<T1>,
      class D2 = std::remove_cvref_t<T2>>
    requires has_common_algebra_type_v<D1, D2> and
                 (std::is_same_v<zero_constant, D1> !=
                  std::is_same_v<zero_constant, D2>)
  friend constexpr auto operator+(T1&& lhs, T2&& rhs)
      -> std::conditional_t<std::is_same_v<zero_constant, D2>, D1, D2>
  {
    if constexpr (std::is_same_v<zero_constant, D2>) {
      return std::forward<T1>(lhs);
    } else {
      return std::forward<T2>(rhs);
    }
  }

  /// addition
  /// @returns `zero_constant` value
  ///
  friend constexpr auto
  operator+(zero_constant, zero_constant) noexcept -> zero_constant
  {
    return {};
  }
};

}  // namespace rigid_geometric_algebra
