#pragma once

#include "rigid_geometric_algebra/detail/define_prioritized_overload.hpp"
#include "rigid_geometric_algebra/detail/invoke_prioritized_overload.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"
#include "rigid_geometric_algebra/detail/priority_list.hpp"

#include <functional>
#include <type_traits>

namespace rigid_geometric_algebra::detail {

/// subtraction operation defined as addition and negation
///
struct default_subtraction
{
  template <class T1, class T2>
  static constexpr auto operator()(T1&& t1, T2&& t2)
  {
    return std::forward<T1>(t1) + (-std::forward<T2>(t2));
  }
};

/// synthesizes subtraction from addition and negation
/// @tparam D derived type
///
/// Defines a subtraction operation for derived type D if
/// +: T1 X T2 -> R
/// is defined, where T1 or T2 is D.
///
template <class D>
class derive_subtraction
{
public:
  template <class T>
  static constexpr auto is_derived_reference_v =
      std::is_same_v<D, std::remove_cvref_t<T>>;

  /// subtraction
  ///
  template <class T1, class T2>
    requires (is_derived_reference_v<T1> or is_derived_reference_v<T2>) and
                 std::is_invocable_v<std::plus<>, T1, T2> and
                 define_prioritized_overload_v<
                     priority_for<std::minus<>, derive_subtraction<>>,
                     overload<std::minus<>, T1, T2>,
                     default_subtraction>
  friend constexpr auto
  operator-(T1&& t1, T2&& t2) -> std::invoke_result_t<std::plus<>, T1, T2>
  {
    return invoke_prioritized_overload<std::minus<>>(
        std::forward<T1>(t1), std::forward<T2>(t2));
  }

  /// equality comparison
  ///
  friend auto operator==(const derive_subtraction&, const derive_subtraction&)
      -> bool = default;
};

}  // namespace rigid_geometric_algebra::detail
