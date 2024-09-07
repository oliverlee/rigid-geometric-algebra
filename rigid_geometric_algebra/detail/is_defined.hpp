#pragma once

#include "rigid_geometric_algebra/detail/type_list.hpp"

#include <tuple>

namespace rigid_geometric_algebra::detail {

/// boolean state for a type
/// @tparam Op type, usually denoting an operation
///
/// Flag used to store boolean state. The value associated with this flag is
/// if the expression `is_defined(flag)` is well formed. On definition of
/// `flag`, `is_defined(flag)` has deduced return type, but no definition to
/// deduce a type, so a call is ill-formed.
///
template <class Op>
struct flag
{
  friend constexpr auto is_defined(flag);
};

/// sets the boolean state for a flag
/// @tparam Op type, usually denoting an operation
///
/// Helper used to set the state of `flag<Op>` to `true`. This type provides a
/// definition for friend function `is_defined(flag<Op>)` in the enclosing
/// namespace after it is instantiated for the first time. Afterwards, the
/// expression `is_defined(flag<Op>)` is well formed.
///
template <class Op>
struct set_defined
{
  friend constexpr auto is_defined(flag<Op>) {}
};

/// checks the state of a flag
/// @tparam Op type, usually denoting an operation
/// @tparam always_eval unique value used to force evaluation of
///     `is_defined(flag<Op>)`
///
/// Checks the state of the flag. On the "first" call to this function,
/// instantiates `set_defined<Op>` and returns `false`. Subsequent calls return
/// `true`.
///
/// @see https://stackoverflow.com/a/58200261
///
template <class Op, auto always_eval = [] {}>
consteval auto check_defined()
{
  if constexpr (requires { is_defined(flag<Op>{}); }) {
    return true;
  } else {
    std::ignore = set_defined<Op>{};
    return false;
  }
}

/// checks if an operation is defined
/// @tparam Op type, usually denoting an operation
/// @tparam state deduced
///
/// Stateful boolean to check the state (true or false) associated with `Op`.
/// On first use, the value is `false`. On subsequent uses, the value is `true`.
///
/// ~~~{.cpp}
/// static_assert(not is_defined_v<op<std::minus<>, T, T>>);
/// static_assert(is_defined_v<op<std::minus<>, T, T>>);
/// ~~~
///
/// @see https://b.atch.se/posts/constexpr-counter/
/// @see https://mc-deltat.github.io/articles/stateful-metaprogramming-cpp20
/// @see
/// https://stackoverflow.com/questions/44267673/is-stateful-metaprogramming-ill-formed-yet
/// @see https://cplusplus.github.io/CWG/issues/2118.html
///
template <class Op, auto state = check_defined<Op>()>
inline constexpr auto is_defined_v = state;

/// helper used to identify an operation under test
/// @tparam Ts types, typically a function object type followed by arguments
/// types
/// @relates is_defined_v
///
template <class... Ts>
using op = type_list<Ts...>;

}  // namespace rigid_geometric_algebra::detail
