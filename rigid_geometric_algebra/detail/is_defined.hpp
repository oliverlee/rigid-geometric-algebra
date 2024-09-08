#pragma once

#include "rigid_geometric_algebra/detail/is_specialization_of.hpp"

#include <tuple>

namespace rigid_geometric_algebra::detail {

/// boolean state for a type
/// @tparam Ts tag types, usually denoting an overload
///
/// Flag used to store boolean state. The value associated with this flag is
/// if the expression `is_defined(flag)` is well formed. On definition of
/// `flag`, `is_defined(flag)` has deduced return type, but no definition to
/// deduce a type, so a call is ill-formed.
///
template <class... Ts>
struct flag
{
  friend consteval auto is_defined(flag);
};

/// sets the state of a flag to a value
/// @tparam Flag flag to set
///
/// Helper used to set the state of `flag<Op>`. This type provides a definition
/// for friend function `is_defined(flag<Op>)` in the enclosing namespace after
/// it is instantiated for the first time. Afterwards, the expression
/// `is_defined(flag<Op>)` is well formed and returns `value`.
///
template <class Flag, auto value>
  requires is_specialization_of_v<Flag, flag>
struct define_value
{
  friend consteval auto is_defined(Flag) { return value; }
};

/// sets the flag defined state
/// @tparam Ov tag type, usually denoting an overload
/// @tparam always_eval unique value used to force evaluation of
///     `is_defined(flag<Op>)`
///
/// Set the flag defined state to `true` and returns the previous defined state
/// value. On the "first" call to this function, instantiates
///`set_defined<Op>` and returns `false`. Subsequent calls return `true`.
///
/// @see https://stackoverflow.com/a/58200261
///
template <class Ov, auto always_eval = [] {}>
consteval auto define()
{
  if constexpr (requires { is_defined(flag<Ov>{}); }) {
    return true;
  } else {
    std::ignore = define_value<flag<Ov>, nullptr>{};
    return false;
  }
}

/// checks if an overload is defined
/// @tparam Ov tag type, usually denoting an overload
/// @tparam state deduced
///
/// Stateful boolean to check the state (`true` or `false`) associated with
/// `Op`. On first use, the value is `false`. On subsequent uses, the value is
/// `true`.
///
/// ~~~{.cpp}
/// static_assert(not is_defined_v<overload<std::minus<>, T, T>>);
/// static_assert(is_defined_v<overload<std::minus<>, T, T>>);
/// ~~~
///
/// @see https://b.atch.se/posts/constexpr-counter/
/// @see https://mc-deltat.github.io/articles/stateful-metaprogramming-cpp20
/// @see
/// https://stackoverflow.com/questions/44267673/is-stateful-metaprogramming-ill-formed-yet
/// @see https://cplusplus.github.io/CWG/issues/2118.html
///
template <class Ov, auto state = define<Ov>()>
inline constexpr auto is_defined_v = state;

}  // namespace rigid_geometric_algebra::detail
