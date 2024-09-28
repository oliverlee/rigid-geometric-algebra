#pragma once

#include "rigid_geometric_algebra/blade_ordering.hpp"
#include "rigid_geometric_algebra/blade_type_from.hpp"
#include "rigid_geometric_algebra/common_algebra_type.hpp"
#include "rigid_geometric_algebra/detail/array_subset.hpp"
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
/// @tparam Bs blade types
///
/// Defines static member typedef `type` as a specialization of `type_list` with
/// blade types in ascending order. Duplicate blade types are removed from the
/// returned `type_list`.
///
/// @note Requires:j
/// * `Bs` are all specializations of `blade`
/// * `Bs` have a common algebra type
///
/// @{

template <class... Bs>
  requires (is_blade_v<std::remove_cvref_t<Bs>> and ...) and
           (sizeof...(Bs) == 0 or
            detail::has_type_v<common_algebra_type<Bs...>>)
struct sorted_canonical_blades
{
  using A = common_algebra_type_t<Bs...>;

  static constexpr auto sorted = [] {
    auto values = std::array{blade_ordering{std::type_identity<Bs>{}}...};
    std::ranges::sort(values);
    const auto duplicates = std::ranges::unique(values);
    return detail::array_subset(values, values.size() - duplicates.size());
  }();

  static_assert(sorted.size() != 0);

  template <std::size_t... Is>
  static constexpr auto impl(std::index_sequence<Is...>)
      -> detail::type_list<blade_type_from_t<A, sorted[Is].mask>...>;

  using type = decltype(impl(std::make_index_sequence<sorted.size()>{}));
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
