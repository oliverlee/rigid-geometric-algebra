#pragma once

#include "rigid_geometric_algebra/detail/is_defined.hpp"

#include <functional>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// synthesizes subtraction from addition and negation
/// @tparam D derived type
///
/// Defines a subtraction operation for derived type D if
/// +: T1 X T2 -> R
/// is defined, where T1 or T2 is D.
///
template <class D>
struct derive_subtraction
{
  template <class T>
  static constexpr auto is_derived_reference_v =
      std::is_same_v<D, std::remove_cvref_t<T>>;

  /// subtraction
  ///
  template <class T1, class T2>
    requires (is_derived_reference_v<T1> or is_derived_reference_v<T2>) and
                 std::is_invocable_v<std::plus<>, T1, T2> and
                 (not is_defined_v<op<derive_subtraction<void>, T1, T2>>)
  friend constexpr auto
  operator-(T1&& t1, T2&& t2) -> std::invoke_result_t<std::plus<>, T1, T2>
  {
    return std::forward<T1>(t1) + (-std::forward<T2>(t2));
  }

  /// equality comparison
  ///
  /// defined to allow default definition for derived types
  ///
  friend auto operator==(const derive_subtraction&, const derive_subtraction&)
      -> bool = default;
};

}  // namespace rigid_geometric_algebra::detail
