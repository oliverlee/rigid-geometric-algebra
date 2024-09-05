#pragma once

#include "rigid_geometric_algebra/blade_from_ordering.hpp"
#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/has_type.hpp"
#include "rigid_geometric_algebra/detail/type_list.hpp"
#include "rigid_geometric_algebra/is_blade.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace rigid_geometric_algebra {

/// returns a type list containing canonical blade types in canonical order
///
/// @{

template <class... Bs>
  requires (is_blade_v<Bs> and ...) and
           (sizeof...(Bs) == 0 or
            detail::has_type_v<common_algebra_type<Bs...>>)
struct sorted_canonical_blades
{
  using A = common_algebra_type_t<Bs...>;

  static constexpr auto ordering = [] {
    auto ordering = std::array{blade_ordering<A>{std::type_identity<Bs>{}}...};
    std::ranges::sort(ordering);
    return ordering;
  }();

  using type = decltype([]<std::size_t... Is>(std::index_sequence<Is...>) {
    return detail::type_list<
        blade_from_ordering_t<std::get<Is>(ordering)>...>{};
  }(std::index_sequence_for<Bs...>{}));
};

template <>
struct sorted_canonical_blades<>
{
  using type = detail::type_list<>;
};

template <class... Bs>
using sorted_canonical_blades_t = typename sorted_canonical_blades<Bs...>::type;

/// @}

}  // namespace rigid_geometric_algebra
