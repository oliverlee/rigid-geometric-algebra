#pragma once

#include "rigid_geometric_algebra/blade.hpp"
#include "rigid_geometric_algebra/detail/array_subset.hpp"
#include "rigid_geometric_algebra/detail/indices_array.hpp"
#include "rigid_geometric_algebra/detail/structural_bitset.hpp"

#include <cstddef>
#include <utility>

namespace rigid_geometric_algebra {

/// determine the blade type from a range of dimensions
/// @tparam A algebra type
/// @tparam dimensions random access range of dimensions
///
/// ~~~{.cpp}
/// static_assert(std::is_same_v<
///   blade<A, 0, 2, 1>,
///   blade_type_from_dimensions_t<A, {0, 2, 1}>
/// >);
/// ~~~
///
/// @{

template <
    class A,
    detail::array_subset<std::size_t, algebra_dimension_v<A>> dimensions>
struct blade_type_from_dimensions
{
  using type =
      decltype([]<std::size_t... Is>(
                   std::index_sequence<Is...>) -> blade<A, dimensions[Is]...> {
        return {};
      }(std::make_index_sequence<dimensions.size()>{}));
};

template <
    class A,
    detail::array_subset<std::size_t, algebra_dimension_v<A>> dimensions>
using blade_type_from_dimensions_t =
    typename blade_type_from_dimensions<A, dimensions>::type;

/// @}

/// determine the blade type from a dimension mask
/// @tparam A algebra type
/// @tparam mask range of `bool`
///
/// Determins the `blade` type given an algebra type and a mask of dimensions.
/// Each bool in the mask correspond to a dimension, starting dimension 0.
///
/// @note The `blade` type is always in canonical form.
///
/// @{

template <class A, detail::structural_bitset<algebra_dimension_v<A>> mask>
struct blade_type_from_mask
{
  static constexpr auto dimensions = detail::indices_array<mask>;
  using type = blade_type_from_dimensions_t<A, dimensions>;
};

template <class A, detail::structural_bitset<algebra_dimension_v<A>> mask>
using blade_type_from_mask_t = typename blade_type_from_mask<A, mask>::type;

/// @}

}  // namespace rigid_geometric_algebra
