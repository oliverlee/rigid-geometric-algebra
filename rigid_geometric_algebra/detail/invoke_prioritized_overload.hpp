#pragma once

#include "rigid_geometric_algebra/detail/is_defined.hpp"
#include "rigid_geometric_algebra/detail/overload.hpp"
#include "rigid_geometric_algebra/detail/priority.hpp"
#include "rigid_geometric_algebra/detail/priority_list.hpp"

#include <cstddef>
#include <utility>

namespace rigid_geometric_algebra::detail {

template <class Op, auto always_eval = [] {}>
class invoke_prioritized_overload_fn
{
  template <std::size_t N, class... Args>
  static constexpr auto impl(priority<N>, Args&&... args)
  {
    if constexpr (
        requires { is_defined(flag<priority<N>, overload<Op, Args...>>{}); }) {
      return is_defined(flag<priority<N>, overload<Op, Args...>>{})(
          std::forward<Args>(args)...);
    } else if constexpr (N == 0) {
      static_assert(
          N != 0,  // always false
          "unable to find a valid overload, please review use of "
          "`defined_prioritized_overload`,");
    } else {
      return impl(priority<N - 1>{}, std::forward<Args>(args)...);
    }
  }

public:
  template <class... Args>
  static constexpr auto operator()(Args&&... args)
  {
    return impl(priority<max_priority<Op>>{}, std::forward<Args>(args)...);
  }
};

/// invokes a synthesized overload set with arguments
/// @tparam Op operation tag for an overload set
/// @tparam Args argument types
/// @param args argument value
///
/// Invokes the synthesized overload set associated with
/// `overload<Op, Args...>` with `args...`. This overload set in synthesized by
/// calls to `define_prioritized_overload_v` with different priority values.
///
/// ~~~{.cpp}
/// // invoked within a metaprogram
/// ... define_prioritized_overload_v<0, overload<Op, Args...>, F0> ...
/// ... define_prioritized_overload_v<1, overload<Op, Args...>, F1> ...
/// ... define_prioritized_overload_v<2, overload<Op, Args...>, F2> ...
///
/// ...
///
/// // calls F2{}(std::forward<Args>(args)...);
/// invoke_prioritized_overload<Op>(args...);
/// ~~~
///
/// @see define_prioritized_overload
/// @see overload
///
template <class Op, auto always_eval = [] {}>
inline constexpr auto invoke_prioritized_overload =
    invoke_prioritized_overload_fn<Op>{};

}  // namespace rigid_geometric_algebra::detail
