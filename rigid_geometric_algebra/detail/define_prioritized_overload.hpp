#pragma once

#include "rigid_geometric_algebra/detail/is_defined.hpp"
#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"
#include "rigid_geometric_algebra/detail/priority.hpp"

#include <cstddef>
#include <tuple>

namespace rigid_geometric_algebra::detail {

/// implementation helper to add an operation to a prioritized overload set
/// @tparam N priority value
/// @tparam Ov overload tag
/// @tparam F implementation function object
///
/// Sets `F` as the definition for overload `Ov` with priority `N`.
///
/// The set overload should not be invoked directly, but through
/// `invoke_prioritized_overload`.
///
/// @note Mandates:
/// * no other calls to `define_with_priority` with the same `N` and `Op`.
///
template <std::size_t N, class Ov, class F, auto always_eval = [] {}>
consteval auto define_with_priority()
{
  using flag_type = flag<priority<N>, Ov>;

  if constexpr (requires { is_defined(flag_type{}); }) {
    static_assert(
        std::is_same_v<F, decltype(is_defined(flag_type{}))>,
        "multiple definitions for the same priority and overload must use the "
        "same implementation");
    return false;
  } else {
    std::ignore = define_value<flag_type, F{}>{};
    return true;
  }
}

/// add an operation to a synthesized overload set with priority
/// @tparam N priority value
/// @tparam Ov overload tag
/// @tparam F implementation function object
/// @tparam state deduced
///
/// Stateful boolean to define a overload of a synthesized overload set with a
/// priority value.
///
/// Returns `true` on first call for a given `Ov`, regardless of priority value.
/// Subsequent calls return `false`. This allows use in a `requires` clause to
/// avoid multiple definition errors in expected use cases.
///
/// `Ov` must be a specialization of `overload` and is a tag that represents
/// the overload set for an operation `Op` with arguments `Args...` and `F` is
/// the implementation assigned to priority `N`.
///
/// Setup involves using this variable template in the `requires` clause of a
// function template and the function body must return the result of
// `invoke_derived`.
///
/// ~~~{.cpp}
/// template <class... Args>
/// requires define_prioritized_overload_v<0, overload<Op, Args...>, F0>
///   and ...
/// friend constexpr auto operator_X(Args&& args)
/// {
///     return invoke_derived<Op>(std::forward<Args>(args)...);
/// }
/// ...
/// template <class... Args>
/// requires define_prioritized_overload_v<1, overload<Op, Args...>, F1>
///   and ...
/// friend constexpr auto operator_X(Args&& args)
/// {
///     return invoke_derived<Op>(std::forward<Args>(args)...);
/// }
/// ~~~
///
/// @see invoke_prioritized_overload
/// @see overload
///
template <
    std::size_t N,
    class Ov,
    class F,
    auto state = define_with_priority<N, Ov, F>() and (not is_defined_v<Ov>)>
  requires is_specialization_of_v<Ov, overload>
inline constexpr auto define_prioritized_overload_v = state;

}  // namespace rigid_geometric_algebra::detail
