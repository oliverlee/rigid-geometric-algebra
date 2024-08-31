#pragma once

#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <type_traits>

namespace rigid_geometric_algebra {

/// determines if a sequence of blades matches canonical order
/// @tparam Bs blade types
///
/// Returns `true` if blades are listed in canonical sequence order, otherwise
/// `false`.
///
/// @note Requires:
/// * each type in `Bs` is a specialization of `blade`
/// * each type in `Bs` is in canonical form
/// * `Bs...` contain a common `algebra`
///

namespace detail {

template <class... Bs>
  requires (is_blade_v<Bs> and ...) and
           (std::is_same_v<Bs, typename Bs::canonical_type> and ...) and
           ((sizeof...(Bs) == 0) or
            detail::has_type_v<common_algebra_type<Bs...>>)
class is_canonical_blade_order_fn
{
public:
  constexpr static auto operator()() -> bool
  {
    if constexpr (sizeof...(Bs) == 0) {
      return true;
    } else {
      using A = common_algebra_type_t<Bs...>;
      const auto data = std::array<blade_ordering<A>, sizeof...(Bs)>{
          std::type_identity<Bs>{}...};
      return std::ranges::is_sorted(data, std::ranges::less_equal{});
    }
  }
};

}  // namespace detail

template <class... Bs>
inline constexpr auto is_canonical_blade_order =
    detail::is_canonical_blade_order_fn<Bs...>{};

}  // namespace rigid_geometric_algebra
