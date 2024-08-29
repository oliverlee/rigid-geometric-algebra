#pragma once

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
  ///
  template <class T1, class T2>
    requires std::is_same_v<A, typename T1::algebra_type> and
                 std::is_same_v<A, typename T2::algebra_type> and
                 (std::is_same_v<zero_constant, T1> or
                  std::is_same_v<zero_constant, T2>)
  friend constexpr auto
  operator^(const T1&, const T2&) noexcept -> zero_constant
  {
    return {};
  }
};

}  // namespace rigid_geometric_algebra
