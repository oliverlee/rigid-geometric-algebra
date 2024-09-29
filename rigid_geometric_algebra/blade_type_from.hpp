#pragma once

#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/detail/indices_array.hpp"
#include "rigid_geometric_algebra/is_algebra.hpp"

#include <cstddef>
#include <ranges>
#include <utility>

namespace rigid_geometric_algebra {

/// determine the blade type from a range of dimensions
/// @tparam A algebra type
/// @tparam dimensions random access range of dimensions
///
/// ~~~{.cpp}
/// static_assert(std::is_same_v<
///   blade<A, 0, 2, 1>,
///   blade_type_from_dimensions_t<A, std::array{0, 2, 1}>
/// >);
/// ~~~
///
/// @{

template <class A, std::ranges::random_access_range auto dimensions>
  requires is_algebra_v<A>
struct blade_type_from_dimensions
{
  using type =
      decltype([]<std::size_t... Is>(
                   std::index_sequence<Is...>) -> blade<A, dimensions[Is]...> {
        return {};
      }(std::make_index_sequence<dimensions.size()>{}));
};

template <class A, auto dimensions>
using blade_type_from_dimensions_t =
    typename blade_type_from_dimensions<A, dimensions>::type;

/// @}

/// determine the blade type from a dimension mask or range of dimensions
/// @tparam A algebra type
/// @tparam mask_or_dimensions forward range of `bool` or random access range
/// of dimensions
///
/// Determins the `blade` type given an algebra type and either a mask or
/// dimensions. If `mask_or_dimensions` has a range value type of `bool`, it is
/// interpreted as a mask. Otherwise, it is interpreted as dimensions.
///
/// @note The `blade` type given a mask is always in canonical form.
///
/// @{

template <class A, std::ranges::forward_range auto mask_or_dimensions>
  requires is_algebra_v<A>
struct blade_type_from
{
  using type = typename decltype([] {
    if constexpr (
        std::is_same_v<
            bool,
            std::ranges::range_value_t<decltype(mask_or_dimensions)>>) {
      static constexpr auto dimensions =
          detail::indices_array<mask_or_dimensions>;
      return blade_type_from_dimensions<A, dimensions>{};
    } else {
      return blade_type_from_dimensions<A, mask_or_dimensions>{};
    }
  }())::type;
};

template <class A, auto mask_or_dimensions>
using blade_type_from_t = typename blade_type_from<A, mask_or_dimensions>::type;

/// @}

}  // namespace rigid_geometric_algebra
